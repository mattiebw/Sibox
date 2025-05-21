#include "siboxpch.h"
#include "Physics/PhysicsScene.h"

#include "Core/Application.h"

PhysicsScene::~PhysicsScene()
{
    // Note that deleting nullptr is safe.
    delete[] m_SortedBodyArray;
}

void PhysicsScene::Update()
{
    f32 delta = static_cast<f32>(Application::Get()->GetDeltaTime());
    if (delta == 0)
        return;

    // First, apply gravity to each body.
    for (Body *body : m_Bodies)
    {
        // Values we'll be using for the physics simulation.
        // We use inverse masses to represent infinite masses as an inverse mass of 0 - so, we could get a division by 0
        // here! Obviously, this'll cause a NaN, and then we'll get NaN propogation - however, we return out of ApplyLinearImpulse
        // if our inverse mass is 0, so that won't happen.
        const f32 mass = 1.0f / body->InverseMass;

        // Apply gravity
        Vector3F gravity = Vector3F{0.0f, -9.81f, 0.0f} * mass * delta;
        body->ApplyLinearImpulse(gravity);
    }

    // Perform the braod phase collision check.
    std::vector<CollisionPair> pairs;
    BroadPhase(const_cast<const Body**>(m_Bodies.data()), m_Bodies.size(), pairs, delta);

    // Narrow phase collision detection.
    s32           numContacts        = 0;
    const s32     maxContacts        = m_Bodies.size() * m_Bodies.size();
    constexpr s32 maxSizeForStack    = 256;
    BodyContact*  contacts           = nullptr;
    bool          contactsIsMalloced = maxContacts > maxSizeForStack;

    // Stack memory is faster, but limited in space. So we select which to use depending on how many contacts
    // we need to allocate.
    if (contactsIsMalloced)
        contacts = new BodyContact[maxContacts];
    else
        contacts = static_cast<BodyContact*>(alloca(sizeof(BodyContact) * maxContacts));

    // for (s32 i = 0; i < m_Bodies.size(); i++)
    // {
    //     for (s32 j = i + 1; j < m_Bodies.size(); j++)
    //     {
    //         Body& bodyA = *m_Bodies[i];
    //         Body& bodyB = *m_Bodies[j];
    //
    //         // Ignore collisions between static bodies.
    //         if (bodyA.InverseMass == 0.0f && bodyB.InverseMass == 0.0f)
    //             continue;
    //
    //         if (Body::Intersects(bodyA, bodyB, contacts[numContacts], delta))
    //             numContacts++;
    //     }
    // }

    for (CollisionPair& pair : pairs)
    {
        Body* bodyA = m_Bodies[pair.BodyAID];
        Body* bodyB = m_Bodies[pair.BodyBID];

        // Ignore collisions between static bodies.
        if (bodyA->InverseMass == 0.0f && bodyB->InverseMass == 0.0f)
            continue;

        if (Body::Intersects(*bodyA, *bodyB, contacts[numContacts], delta))
            numContacts++;
    }

    // Ensure that contacts are in order of TOI, so we can resolve them properly.
    if (numContacts > 1)
        qsort(contacts, numContacts, sizeof(BodyContact), CompareContactsForQSort);

    f32 accumulatedDT = 0.0f;
    for (int i = 0; i < numContacts; i++)
    {
        BodyContact& contact = contacts[i];
        const f32 requiredDelta = contact.TimeOfImpact - accumulatedDT;
        
        for (Body* body : m_Bodies)
            body->Update(requiredDelta);

        ResolveContact(contact);
        accumulatedDT += requiredDelta;
    }

    if (contactsIsMalloced)
        delete[] contacts;
    
    const f32 remainingTime = delta - accumulatedDT;
    if (remainingTime > 0)
    {
        for (Body* body : m_Bodies)
            body->Update(remainingTime);
    }
}

void PhysicsScene::AddBody(Body *body)
{
    m_Bodies.push_back(body);
}

void PhysicsScene::RemoveBody(Body *body)
{
    for (s32 i = static_cast<s32>(m_Bodies.size()) - 1; i >= 0; i--)
    {
        if (m_Bodies[i] == body)
        {
            m_Bodies.erase(m_Bodies.begin() + i);
            break;
        }
    }
}

void PhysicsScene::ClearBodies()
{
    for (Body *body : m_Bodies)
        delete body;
    m_Bodies.clear();
}

void PhysicsScene::ClearDynamicBodies()
{
    for (int i = static_cast<int>(m_Bodies.size()) - 1; i >= 0; i--)
    {
        if (m_Bodies[i]->InverseMass == 0.0f)
            continue;

        delete m_Bodies[i];
        m_Bodies.erase(m_Bodies.begin() + i);
    }
}

FirstContact PhysicsScene::CompareContacts(const BodyContact& a, const BodyContact& b)
{
    if (a.TimeOfImpact < b.TimeOfImpact)
        return FirstContact::ContactA;

    if (a.TimeOfImpact > b.TimeOfImpact)
        return FirstContact::ContactB;

    return FirstContact::SameTime;
}

// This is our initial collision resolution function.
// We're just resolving an interpenetration, which is not really how we want to do it, but our initial implementation.
void PhysicsScene::ResolveContact(const BodyContact &contact)
{
    // Get our bodies and their masses, just to make the code cleaner.
    Body *bodyA = contact.BodyA;
    Body *bodyB = contact.BodyB;
    const f32 bodyAInverseMass = bodyA->InverseMass;
    const f32 bodyBInverseMass = bodyB->InverseMass;
    const Vector3F worldSpacePointOnA = bodyA->BodySpaceToWorldSpace(contact.BodySpacePointOnA);
    const Vector3F worldSpacePointOnB = bodyB->BodySpaceToWorldSpace(contact.BodySpacePointOnB);

    // Get our elasticity value - for now, we just multiply the two bodies' elasticity values together.
    const f32 elasticity = bodyA->Elasticity * bodyB->Elasticity;

    // Get our inertia matrices.
    const Matrix3x3F inverseInertiaWorldSpaceA = bodyA->GetInverseInertiaTensorWorldSpace();
    const Matrix3x3F inverseInertiaWorldSpaceB = bodyB->GetInverseInertiaTensorWorldSpace();

    // Distances from contact point and center of mass for both bodies.
    const Vector3F centerToContactPointA = worldSpacePointOnA - bodyA->GetCenterOfMassWorldSpace();
    const Vector3F centerToContactPointB = worldSpacePointOnB - bodyB->GetCenterOfMassWorldSpace();

    // Get the angular force for each body.
    // Consider the equation:
    //   new angular velocity =
    //      angular velocity
    //      + (inverse inertia tensor * (vector from center of mass to point of contact x contact normal) * impulse vector)
    const Vector3F angularForceA = (inverseInertiaWorldSpaceA * centerToContactPointA.Cross(contact.NormalWorldSpace)).
        Cross(centerToContactPointA);
    const Vector3F angularForceB = (inverseInertiaWorldSpaceB * centerToContactPointB.Cross(contact.NormalWorldSpace)).
        Cross(centerToContactPointB);
    const f32 angularFactor = (angularForceA + angularForceB).Dot(contact.NormalWorldSpace);

    // World space velocity (motion + rotation).
    const Vector3F velocityA = bodyA->LinearVelocity + bodyA->AngularVelocity.Cross(centerToContactPointA);
    const Vector3F velocityB = bodyB->LinearVelocity + bodyB->AngularVelocity.Cross(centerToContactPointB);

    // Calculate the collision impulse.
    const Vector3F velocityDiff = velocityA - velocityB;
    const float impulse = (1.0f + elasticity) * velocityDiff.Dot(contact.NormalWorldSpace, false) / (bodyAInverseMass +
        bodyBInverseMass + angularFactor);
    const Vector3F impulseVector = contact.NormalWorldSpace * impulse;

    bodyA->ApplyImpulse(worldSpacePointOnA, -impulseVector);
    bodyB->ApplyImpulse(worldSpacePointOnB, impulseVector);

    // Now, we calculate the impulse caused by friction.
    // As with elasticity, we'll just multiply the two together for an approximation of the collision friction.
    const f32 friction = bodyA->Friction * bodyB->Friction;

    // Calculate the normal.
    // This'll give us a normal scaled by how much the velocity is moving into the object that is being collided with.
    const Vector3F velocityNormal = contact.NormalWorldSpace * contact.NormalWorldSpace.Dot(velocityDiff);
    
    // And the tangent:
    const Vector3F velocityTangent = velocityDiff - velocityNormal;
    Vector3F relativeVelocityTangent = velocityTangent;
    relativeVelocityTangent.Normalize();

    const Vector3F inertiaA = (inverseInertiaWorldSpaceA * centerToContactPointA.Cross(relativeVelocityTangent)).Cross(centerToContactPointA);
    const Vector3F inertiaB = (inverseInertiaWorldSpaceB * centerToContactPointB.Cross(relativeVelocityTangent)).Cross(centerToContactPointB);
    const f32 inverseInertia = (inertiaA + inertiaB).Dot(relativeVelocityTangent);

    const f32 reducedMass = 1.0f / (bodyAInverseMass + bodyBInverseMass + inverseInertia);
    const Vector3F impulseFriction = velocityTangent * reducedMass * friction;

    bodyA->ApplyImpulse(worldSpacePointOnA, -impulseFriction);
    bodyB->ApplyImpulse(worldSpacePointOnB, impulseFriction);

    if (contact.TimeOfImpact == 0.0f)
    {
        // We move the bodies apart, and the amount each body moves is based on their mass relative to the other body.
        const float aWeight = bodyA->InverseMass / (bodyA->InverseMass + bodyB->InverseMass);
        const float bWeight = bodyB->InverseMass / (bodyA->InverseMass + bodyB->InverseMass);

        // Penetration is a vector representing how far the two bodies are penetrating into each other.
        Vector3F penetration = contact.WorldSpacePointOnB - contact.WorldSpacePointOnA;

        bodyA->Position += penetration * aWeight;
        bodyB->Position -= penetration * bWeight;
    }
}

s32 PhysicsScene::CompareSAP(const void *a, const void *b)
{
    const PseudoBody* bodyA = static_cast<const PseudoBody*>(a);
    const PseudoBody* bodyB = static_cast<const PseudoBody*>(b);

    if (bodyA->Value < bodyB->Value)
        return -1;
    return 1;
}

void PhysicsScene::SortBodiesBounds(const Body **bodies, const s32 bodyCount, PseudoBody *sortedArray, const f32 delta)
{
    Vector3F axis = {1.0f};
    axis.Normalize();

    for (s32 i = 0; i < bodyCount; i++)
    {
        // Get the body and it's initial bounds.
        const Body *body = bodies[i];
        BoundsF bounds = body->GetShape().GetBounds(body->Position, body->Rotation);

        // Add the velocity to the bounds to account for the movement of the body.
        bounds.Expand(bounds.Min + body->LinearVelocity * delta);
        bounds.Expand(bounds.Max + body->LinearVelocity * delta);

        // Add a small epsilon to the bounds to make sure we catch possible collisions.
        constexpr float epsilon = 0.01f;
        bounds.Expand(bounds.Min + Vector3F(-1.0f) * epsilon);
        bounds.Expand(bounds.Min + Vector3F( 1.0f) * epsilon);

        // Fill in our pseudo body array.
        sortedArray[i * 2 + 0].ID = i;
        sortedArray[i * 2 + 0].Value = axis.Dot(bounds.Min);
        sortedArray[i * 2 + 0].IsMin = true;
        
        sortedArray[i * 2 + 1].ID = i;
        sortedArray[i * 2 + 1].Value = axis.Dot(bounds.Max);
        sortedArray[i * 2 + 1].IsMin = false;
    }

    // Sort the bodies.
    qsort(sortedArray, bodyCount * 2, sizeof(PseudoBody), CompareSAP);
}

void PhysicsScene::BuildPairs(std::vector<CollisionPair> &pairs, const PseudoBody *sortedBodies, const s32 bodyCount)
{
    pairs.clear();

    for (s32 i = 0; i < bodyCount * 2; i++)
    {
        const PseudoBody& pseudoBodyA = sortedBodies[i];
        if (!pseudoBodyA.IsMin)
            continue;

        CollisionPair pair;
        pair.BodyAID = pseudoBodyA.ID;

        for (s32 j = i + 1; j < bodyCount * 2; j++)
        {
            const PseudoBody& pseudoBodyB = sortedBodies[j];
            if (pseudoBodyB.ID == pseudoBodyA.ID)
            {
                // We've found the other side of the bounds of body A, so we're done looking for pairs for body A.
                break;
            }

            if (!pseudoBodyB.IsMin)
                continue;

            pair.BodyBID = pseudoBodyB.ID;
            pairs.push_back(pair);
        }
    }
}

void PhysicsScene::SweepAndPrune1D(const Body **bodies, const s32 bodyCount, std::vector<CollisionPair> &outputPairs,
    const f32 delta)
{
    if (m_SortedBodyArraySize < bodyCount * 2)
    {
        delete[] m_SortedBodyArray;
        m_SortedBodyArray = new PseudoBody[bodyCount * 2];
        m_SortedBodyArraySize = bodyCount * 2;
    }

    SortBodiesBounds(bodies, bodyCount, m_SortedBodyArray, delta);
    BuildPairs(outputPairs, m_SortedBodyArray, bodyCount);
}

void PhysicsScene::BroadPhase(const Body **bodies, const s32 bodyCount, std::vector<CollisionPair> &outputPairs,
    const f32 delta)
{
    outputPairs.clear();
    SweepAndPrune1D(bodies, bodyCount, outputPairs, delta);
}

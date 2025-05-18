#include "siboxpch.h"
#include "Physics/PhysicsScene.h"

#include "Core/Application.h"

void PhysicsScene::Update()
{
	f64 delta = Application::Get()->GetDeltaTime();

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

		// Horrible brute force collision check for now.
		for (Body *otherBody : m_Bodies)
		{
			// Ignore self
			if (body == otherBody)
				continue;
			
			// Ignore body pairs that both have infinite mass.
			if (body->InverseMass == 0.0f && otherBody->InverseMass == 0.0f)
				continue;

			// If two bodys intersect, we'll just make them stop moving for now.
			BodyContact contact;
			if (Body::Intersects(*body, *otherBody, contact))
			{
				ResolveContact(contact);
			}
		}

		// Apply linear velocity
		body->Position += body->LinearVelocity * static_cast<float>(delta);
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

// This is our initial collision resolution function.
// We're just resolving an interpenetration, which is not really how we want to do it, but our initial implementation.
void PhysicsScene::ResolveContact(BodyContact &contact)
{
	// Get our bodies, just to make the code cleaner.
	Body* bodyA = contact.BodyA;
	Body* bodyB = contact.BodyB;

	// Zero our velocities. Not how it actually works, but this'll do for now.
	bodyA->LinearVelocity.Zero();
	bodyB->LinearVelocity.Zero();

	const float aWeight = bodyA->InverseMass / (bodyA->InverseMass + bodyB->InverseMass);
	const float bWeight = bodyB->InverseMass / (bodyA->InverseMass + bodyB->InverseMass);

	// Penetration is a vector representing how far the two bodies are penetrating into each other.
	const Vector3F penetration = contact.WorldSpacePointOnB - contact.WorldSpacePointOnA;

	bodyA->Position += penetration * aWeight;
	bodyB->Position -= penetration * bWeight;
}

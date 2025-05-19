#include "siboxpch.h"
#include "Physics/Body.h"

Vector3F Body::WorldSpaceToBodySpace(const Vector3F &worldSpace) const
{
	Vector3F    localSpace      = worldSpace - GetCenterOfMassWorldSpace();
	QuaternionF inverseRotation = Rotation.Inverted();
	Vector3F    bodySpace       = inverseRotation.RotatePoint(localSpace);
	return bodySpace;
}

Vector3F Body::BodySpaceToWorldSpace(const Vector3F &bodySpace) const
{
	return GetCenterOfMassWorldSpace() + Rotation.RotatePoint(bodySpace);
}

Vector3F Body::GetCenterOfMassWorldSpace() const
{
	return Position + Rotation.RotatePoint(m_Shape.GetCenterOfMass());
}

Vector3F Body::GetCenterOfMassBodySpace() const
{
	return m_Shape.GetCenterOfMass();
}

Matrix3x3F Body::GetInverseInertiaTensorWorldSpace() const
{
	Matrix3x3F inertiaTensor = m_Shape.GetInertiaTensor();
	inertiaTensor.Invert();
	inertiaTensor *= InverseMass;
	Matrix3x3F orientation = Rotation.ToMat3();
	
	// A couple of notes:
	// - The orientation matrix has a determinant of 1, meaning it is orthogonal. This means that the inverse is the same as the transpose.
	//   Transposing is significantly faster than inverting, so we do that.
	// - What we're doing here is building a matrix that will do the following to an angular impulse:
	//   - Transform the angular impulse from world space to body space.
	//   - Apply the body-space inertia tensor.
	//   - Transform the result back to world space.
	inertiaTensor = orientation * inertiaTensor * orientation.Transpose();
	
	return inertiaTensor;
}

Matrix3x3F Body::GetInverseInertiaTensorBodySpace() const
{
	Matrix3x3F inertiaTensor = m_Shape.GetInertiaTensor();
	inertiaTensor.Invert();
	inertiaTensor *= InverseMass;
	return inertiaTensor;
}

void Body::ApplyLinearImpulse(const Vector3F &impulse)
{
	// We represent masses as inverse masses - there's a couple of reasons for this:
	// 1. We can multiply by the inverse mass instead of dividing by the mass, which is faster on most CPUs.
	// 2. It allows us to easily represent bodies with an infinite mass - an inverse mass of 0 means infinite.
	//    We'd use this for static bodies, which we don't want to move.
	//    For example: a tennis ball hitting the ground; however hard it hits, the Earth *will* move, but such a tiny
	//    amount that we can ignore it.
	if (InverseMass == 0.0f)
		return;

	// Momentum = Mass * Velocity
	// Change in Momentum = Impulse = Mass * Change in Velocity
	// Meaning:
	// Change in Velocity = Impulse / Mass
	LinearVelocity += impulse * InverseMass;
}

void Body::ApplyAngularImpulse(const Vector3F &angularImpulse)
{
	if (InverseMass == 0.0f)
		return;

	// Angular momentum = Inertia * Angular velocity
	//                  = Cross product of position and linear momentum
	// Change in angular momentum = Inertia * change in angular velocity
	// Therefore: change in angular velocity = inverse inertia * cross product of position and impulse
	// We assume that the provided angular impulse is already the cross product.
	AngularVelocity += GetInverseInertiaTensorWorldSpace() * angularImpulse;

	// We limit the angular velocity.
	// MW @todo: Why?
	constexpr f32 maxAngularVelocity = 30.0f;
	if (AngularVelocity.LengthSquared() > maxAngularVelocity * maxAngularVelocity)
	{
		AngularVelocity.Normalize();
		AngularVelocity *= maxAngularVelocity;
	}
}

// Intersection test functions.
namespace
{
	bool SphereIntersectsSphere(Body &bodyA, Body &bodyB, BodyContact &contact)
	{
		contact.BodyA = &bodyA;
		contact.BodyB = &bodyB;

		f32      radiusSum       = bodyA.GetShape().GetRadius() + bodyB.GetShape().GetRadius();
		Vector3F dist            = bodyB.Position - bodyA.Position;
		contact.NormalWorldSpace = dist.Normalized();
		f32 distSquared          = dist.LengthSquared();

		contact.WorldSpacePointOnA = bodyA.Position + contact.NormalWorldSpace * bodyA.GetShape().GetRadius();
		contact.WorldSpacePointOnB = bodyB.Position - contact.NormalWorldSpace * bodyB.GetShape().GetRadius();

		if (distSquared <= radiusSum * radiusSum)
			return true;

		return false;
	}

	bool AABBIntersectsAABB(Body &bodyA, Body &bodyB, BodyContact& contact)
	{
		contact.BodyA = &bodyA;
		contact.BodyB = &bodyB;

		Vector3F minA = bodyA.Position - bodyA.GetShape().GetSize();
		Vector3F maxA = bodyA.Position + bodyA.GetShape().GetSize();
		Vector3F minB = bodyA.Position - bodyB.GetShape().GetSize();
		Vector3F maxB = bodyA.Position + bodyB.GetShape().GetSize();

		if (minA.X > maxB.X || minB.X > maxA.X)
			return false;
		if (minA.Y > maxB.Y || minB.Y > maxA.Y)
			return false;
		if (minA.Z > maxB.Z || minB.Z > maxA.Z)
			return false;
		
		return true;
	}

	bool AABBIntersectsSphere(Body &aabb, Body &sphere, BodyContact& contact)
	{
		contact.BodyA = &aabb;
		contact.BodyB = &sphere;

		Vector3F aabbMin = aabb.Position - aabb.GetShape().GetSize();
		Vector3F aabbMax = aabb.Position + aabb.GetShape().GetSize();

		Vector3F closestPoint = sphere.Position;
		closestPoint.X        = std::max(aabbMin.X, std::min(closestPoint.X, aabbMax.X));
		closestPoint.Y        = std::max(aabbMin.Y, std::min(closestPoint.Y, aabbMax.Y));
		closestPoint.Z        = std::max(aabbMin.Z, std::min(closestPoint.Z, aabbMax.Z));

		return (closestPoint - sphere.Position).LengthSquared() <= sphere.GetShape().GetRadius() * sphere.GetShape().
		                                                                                                  GetRadius();
	}
}

bool Body::Intersects(Body &bodyA, Body &bodyB, BodyContact &contact)
{
	switch (bodyA.m_Shape.GetType())
	{
	case ShapeType::Sphere:
		switch (bodyB.m_Shape.GetType())
		{
		case ShapeType::Sphere:
			// Sphere vs Sphere
			return SphereIntersectsSphere(bodyA, bodyB, contact);
		case ShapeType::AABB:
			// Sphere vs AABB
			return AABBIntersectsSphere(bodyB, bodyA, contact);
		case ShapeType::Invalid:
		default:
			SIBOX_ERROR("Unsupported shape type for intersection test.");
			return false;
		}
	case ShapeType::AABB:
		switch (bodyB.m_Shape.GetType())
		{
		case ShapeType::Sphere:
			// AABB vs Sphere
			return AABBIntersectsSphere(bodyA, bodyB, contact);
		case ShapeType::AABB:
			// AABB vs AABB
			return AABBIntersectsAABB(bodyA, bodyB, contact);
		case ShapeType::Invalid:
		default:
			SIBOX_ERROR("Unsupported shape type for intersection test.");
			return false;
		}
	case ShapeType::Invalid:
	default:
		SIBOX_ERROR("Unsupported shape type for intersection test.");
		return false;
	}
}

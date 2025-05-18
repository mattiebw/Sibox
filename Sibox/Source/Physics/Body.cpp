#include "siboxpch.h"
#include "Physics/Body.h"

Vector3F Body::WorldSpaceToBodySpace(const Vector3F &worldSpace) const
{
	Vector3F localSpace = worldSpace - GetCenterOfMassWorldSpace();
	QuaternionF inverseRotation = Rotation.Inverted();
	Vector3F bodySpace = inverseRotation.RotatePoint(localSpace);
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

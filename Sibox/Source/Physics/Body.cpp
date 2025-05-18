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

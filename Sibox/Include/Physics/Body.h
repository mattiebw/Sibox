#pragma once

#include "Shape.h"
#include "Math/Quaternion.h"

struct Body;

struct BodyContact
{
	Vector3F WorldSpacePointOnA = {};
	Vector3F WorldSpacePointOnB = {};
	Vector3F BodySpacePointOnA  = {};
	Vector3F BodySpacePointOnB  = {};
	Vector3F NormalWorldSpace   = {};

	// Distance between the two bodies at the contact point. Negative means they are penetrating, positive means they are separating.
	f32 SeparationDistance = 0;
	f32 TimeOfImpact       = 0;

	Body *BodyA = nullptr, *BodyB = nullptr;
};

struct Body
{
public:
	explicit Body(const Shape &shape)
		: Position(0, 0, 0),
		  Rotation(0, 0, 0, 1),
		  LinearVelocity(0, 0, 0),
		  InverseMass(1.f),
		  Elasticity(1.f),
		  m_Shape(shape)
	{
	}

	Vector3F    Position;
	QuaternionF Rotation;
	Vector3F    LinearVelocity;
	f32         InverseMass;
	f32         Elasticity;

	NODISCARD const Shape& GetShape() const { return m_Shape; }
	NODISCARD Shape&       GetShape() { return m_Shape; }

	NODISCARD Vector3F WorldSpaceToBodySpace(const Vector3F &worldSpace) const;
	NODISCARD Vector3F BodySpaceToWorldSpace(const Vector3F &bodySpace) const;

	NODISCARD Vector3F GetCenterOfMassWorldSpace() const;
	NODISCARD Vector3F GetCenterOfMassBodySpace() const;

	void ApplyLinearImpulse(const Vector3F &impulse);

	static bool Intersects(Body &bodyA, Body &bodyB, BodyContact &contact);

protected:
	Shape m_Shape;
};

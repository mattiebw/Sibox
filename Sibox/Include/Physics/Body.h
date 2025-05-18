#pragma once

#include "Shape.h"
#include "Math/Quaternion.h"

struct Body
{
public:
	explicit Body(const Shape &shape)
		: Position(0, 0, 0),
		  Rotation(0, 0, 0, 1),
		  LinearVelocity(0, 0, 0),
		  InverseMass(1.f),
		  m_Shape(shape)
	{
	}

	Vector3F    Position;
	QuaternionF Rotation;
	Vector3F    LinearVelocity;
	f32         InverseMass;

	NODISCARD const Shape &GetShape() const { return m_Shape; }
	NODISCARD Shape& GetShape() { return m_Shape; }
	
	NODISCARD Vector3F WorldSpaceToBodySpace(const Vector3F &worldSpace) const;
	NODISCARD Vector3F BodySpaceToWorldSpace(const Vector3F &bodySpace) const;

	NODISCARD Vector3F GetCenterOfMassWorldSpace() const;
	NODISCARD Vector3F GetCenterOfMassBodySpace() const;

	void ApplyLinearImpulse(const Vector3F &impulse);

	static bool Intersects(const Body &bodyA, const Body &bodyB);

protected:
	Shape m_Shape;
};

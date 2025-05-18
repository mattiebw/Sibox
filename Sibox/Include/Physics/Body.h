#pragma once

#include "Shape.h"
#include "Math/Quaternion.h"

struct Body
{
public:
	explicit Body(const Shape &shape)
		: Position(0, 0, 0),
		  Rotation(0, 0, 0, 1),
		  m_Shape(shape)
	{
	}

	Vector3F    Position;
	QuaternionF Rotation;

	NODISCARD Vector3F WorldSpaceToBodySpace(const Vector3F& worldSpace) const;
	NODISCARD Vector3F BodySpaceToWorldSpace(const Vector3F& bodySpace) const;
	
	NODISCARD Vector3F GetCenterOfMassWorldSpace() const;
	NODISCARD Vector3F GetCenterOfMassBodySpace() const;

protected:
	Shape m_Shape;
};

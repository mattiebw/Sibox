#pragma once

#include "Shape.h"
#include "Math/Quaternion.h"

struct Body
{
public:
	explicit Body(const Shape &shape)
		: Position(0, 0, 0),
		  Rotation(0, 0, 0, 1),
		  BodyShape(shape)
	{
	}

	Vector3F    Position;
	QuaternionF Rotation;
	Shape       BodyShape;
	u32         Handle = 0;
};

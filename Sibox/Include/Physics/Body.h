#pragma once

#include "Shape.h"
#include "Math/Quaternion.h"

class Body
{
public:
    Vector3F Position;
    QuaternionF Rotation;
    Shape BodyShape;
    u32 Handle = 0;
};

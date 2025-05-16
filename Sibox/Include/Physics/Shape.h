#pragma once

enum class ShapeType : u8
{
    Invalid,
    Sphere
};

class Shape
{
public:
    static Shape CreateSphere(float radius)
    {
        Shape sphere = Shape(ShapeType::Sphere);
        sphere.Size = radius;
        sphere.CenterOfMass.Zero();
        return sphere;
    }
    
    ShapeType Type = ShapeType::Invalid;
    float Size = 0;
    Vector3F CenterOfMass = { };

private:
    Shape(ShapeType type)
        : Type(type)
    { }
};

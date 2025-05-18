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
		Shape sphere  = Shape(ShapeType::Sphere);
		sphere.m_Size = radius;
		sphere.m_CenterOfMass.Zero();
		return sphere;
	}

	NODISCARD FORCEINLINE ShapeType GetType() const { return m_Type; }
	NODISCARD FORCEINLINE float     GetRadius() const { return m_Size; } // For spheres.
	NODISCARD FORCEINLINE float     GetSize() const { return m_Size; }
	NODISCARD FORCEINLINE Vector3F  GetCenterOfMass() const { return m_CenterOfMass; }

private:
	Shape(ShapeType type)
		: m_Type(type)
	{
	}

	Vector3F  m_CenterOfMass = {};
	float     m_Size         = 0;
	ShapeType m_Type         = ShapeType::Invalid;
};

#pragma once

enum class ShapeType : u8
{
	Invalid,
	Sphere,
	AABB
};

class Shape
{
public:
	static Shape CreateSphere(float radius)
	{
		Shape sphere  = Shape(ShapeType::Sphere);
		sphere.m_Size = Vector3F(radius, 0, 0);
		sphere.m_CenterOfMass.Zero();
		return sphere;
	}

	static Shape CreateAABB(Vector3F extents)
	{
		Shape aabb  = Shape(ShapeType::AABB);
		aabb.m_Size = extents;
		aabb.m_CenterOfMass.Zero();
		return aabb;
	}

	NODISCARD FORCEINLINE ShapeType GetType() const { return m_Type; }
	NODISCARD FORCEINLINE float     GetRadius() const { return m_Size[0]; } // For spheres.
	NODISCARD FORCEINLINE Vector3F  GetSize() const { return m_Size; }
	NODISCARD FORCEINLINE Vector3F  GetCenterOfMass() const { return m_CenterOfMass; }

private:
	Shape(ShapeType type)
		: m_Type(type)
	{
	}

	Vector3F  m_Size         = {};
	Vector3F  m_CenterOfMass = {};
	ShapeType m_Type         = ShapeType::Invalid;
};

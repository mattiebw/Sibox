#include "siboxpch.h"
#include "Physics/Shape.h"

Matrix3x3F Shape::GetInertiaTensor() const
{
	switch (m_Type)
	{
	case ShapeType::Sphere:
		{
			f32 radius  = m_Size[0];
			f32 inertia = (2.0f / 5.0f) * radius * radius;
			return {inertia};
		}
	case ShapeType::AABB:
		{
			f32 x = m_Size[0];
			f32 y = m_Size[1];
			f32 z = m_Size[2];

			f32 inertiaX = (1.0f / 12.0f) * (y * y + z * z);
			f32 inertiaY = (1.0f / 12.0f) * (x * x + z * z);
			f32 inertiaZ = (1.0f / 12.0f) * (x * x + y * y);

			return {inertiaX, inertiaY, inertiaZ};
		}
	case ShapeType::Invalid:
	default:
		SIBOX_ERROR("Invalid shape type for inertia tensor calculation.");
		return Matrix3x3F(-1);
	}
}

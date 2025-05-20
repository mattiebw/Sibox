#pragma once

template <typename T>
class Bounds
{
	Bounds()
	{
		Clear();
	}

	Bounds(const Bounds& other)
		: Min(other.Min), Max(other.Max)
	{ }

	const Bounds& operator=(const Bounds& other)
	{
		Min = other.Min;
		Max = other.Max;
		return *this;
	}
	
	void Clear()
	{
		// Set to extreme values.
		Min = Vector3F( 1e6 );
		Max = Vector3F( -1e6 );
	}

	NODISCARD FORCEINLINE bool Intersects(const Bounds& other) const
	{
		if (Max.X < other.Min.X || Max.Y < other.Min.Y || Max.Z < other.Min.Z)
			return false;

		if (other.Max.X < Min.X || other.Max.Y < Min.Y || other.Max.Z < Min.Z)
			return false;

		return true;
	}

	void Expand(const Vector3<T>* points, s32 pointsNum)
	{
		for (s32 i = 0; i < pointsNum; i++)
			Expand(points[i]);
	}
	
	void Expand(const Vector3<T>& point)
	{
		if (point.X < Min.X)
			Min.X = point.X;
		if (point.Y < Min.Y)
			Min.Y = point.Y;
		if (point.Z < Min.Z)
			Min.Z = point.Z;

		if (point.X > Max.X)
			Max.X = point.X;
		if (point.Y > Max.Y)
			Max.Y = point.Y;
		if (point.Z > Max.Z)
			Max.Z = point.Z;
	}
	
	void Expand(const Bounds<T>& other)
	{
		Expand(other.Min);
		Expand(other.Max);
	}

	NODISCARD FORCEINLINE T XWidth() const { return Max.X - Min.X; }
	NODISCARD FORCEINLINE T YWidth() const { return Max.Y - Min.Y; }
	NODISCARD FORCEINLINE T ZWidth() const { return Max.Z - Min.Z; }
	
public:
	Vector3<T> Min;
	Vector3<T> Max;
};

#pragma once

template <typename T = float>
	requires std::is_arithmetic_v<T>
struct TRect
{
	Vector2<T> Position;
	Vector2<T> Size;

	TRect(Vector2<T> position, Vector2<T> size)
		: Position(position), Size(size)
	{
	}

	TRect(T x, T y, T width, T height)
		: Position(Vector2<T>(x, y)), Size(Vector2<T>(width, height))
	{
	}

	NODISCARD FORCEINLINE bool OverlapsWith(const TRect &other) const
	{
		return Position.x < other.Position.x + other.Size.x && // Not to the right of the other rectangle
			Position.x + Size.x > other.Position.x &&          // Not to the left of the other rectangle
			Position.y < other.Position.y + other.Size.y &&    // Not below the other rectangle
			Position.y + Size.y > other.Position.y;            // Not above the other rectangle
	}

	NODISCARD FORCEINLINE bool OverlapsWith(T x, T y, T width, T height) const
	{
		return Position.x < x + width && // Not to the right of the other rectangle
			Position.x + Size.x > x &&   // Not to the left of the other rectangle
			Position.y < y + height &&   // Not below the other rectangle
			Position.y + Size.y > y;     // Not above the other rectangle
	}

	NODISCARD FORCEINLINE bool ContainsRect(const TRect &other) const
	{
		return Position.x <= other.Position.x && Position.x + Size.x >= other.Position.x + other.Size.x
			&& Position.y <= other.Position.y && Position.y + Size.y >= other.Position.y + other.Size.y;
	}

	NODISCARD FORCEINLINE bool ContainsPoint(const Vector2<T> &point) const
	{
		return Position.x <= point.x && Position.x + Size.x >= point.x
			&& Position.y <= point.y && Position.y + Size.y >= point.y;
	}

	NODISCARD FORCEINLINE Vector2<T> GetCenter() const
	{
		return Position + (Size / 2.0f);
	}
};

using FRect = TRect<float>;
using DRect = TRect<double>;
using IRect = TRect<int>;

class MathUtil
{
public:
	NODISCARD static glm::mat4 CreateTransformationMatrix(const Vector3f &translation, const Vector3f &rotation,
	                                                      const Vector3f &scale);

	NODISCARD FORCEINLINE static float LerpSmooth(float a, float b, float r, float delta)
	{
		return (a - b) * pow(r, delta) + b;
	}

	NODISCARD FORCEINLINE static Vector2f LerpSmooth(Vector2f a, Vector2f b, float r, float delta)
	{
		return {LerpSmooth(a.X, b.X, r, delta), LerpSmooth(a.Y, b.Y, r, delta)};
	}

	NODISCARD FORCEINLINE static Vector3f LerpSmooth(const Vector3f &a, const Vector3f &b, float r, float delta)
	{
		return {LerpSmooth(a.X, b.X, r, delta), LerpSmooth(a.Y, b.Y, r, delta), LerpSmooth(a.Z, b.Z, r, delta)};
	}
};

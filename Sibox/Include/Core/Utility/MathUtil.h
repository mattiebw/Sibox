#pragma once

template <typename T = f32>
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
		return Position.X < other.Position.X + other.Size.X && // Not to the right of the other rectangle
			Position.X + Size.X > other.Position.X &&          // Not to the left of the other rectangle
			Position.Y < other.Position.Y + other.Size.Y &&    // Not below the other rectangle
			Position.Y + Size.Y > other.Position.Y;            // Not above the other rectangle
	}

	NODISCARD FORCEINLINE bool OverlapsWith(T x, T y, T width, T height) const
	{
		return Position.X < x + width && // Not to the right of the other rectangle
			Position.X + Size.X > x &&   // Not to the left of the other rectangle
			Position.Y < y + height &&   // Not below the other rectangle
			Position.Y + Size.Y > y;     // Not above the other rectangle
	}

	NODISCARD FORCEINLINE bool ContainsRect(const TRect &other) const
	{
		return Position.X <= other.Position.X && Position.X + Size.X >= other.Position.X + other.Size.X
			&& Position.Y <= other.Position.Y && Position.Y + Size.Y >= other.Position.Y + other.Size.Y;
	}

	NODISCARD FORCEINLINE bool ContainsPoint(const Vector2<T> &point) const
	{
		return Position.X <= point.X && Position.X + Size.X >= point.X
			&& Position.Y <= point.Y && Position.Y + Size.Y >= point.Y;
	}

	NODISCARD FORCEINLINE Vector2<T> GetCenter() const
	{
		return Position + (Size / 2.0f);
	}
};

using RectF = TRect<f32>;
using RectD = TRect<f64>;
using RectI = TRect<int>;

class MathUtil
{
public:
	template<typename T>
	NODISCARD FORCEINLINE static T DegreesToRadians(T degrees)
	{
		return degrees * static_cast<T>(0.017453292519943295769236907684886);
	}

	template<typename T>
	NODISCARD FORCEINLINE static T RadiansToDegrees(T radians)
	{
		return radians * static_cast<T>(57.295779513082320876798154814105);
	}
	
	template<typename T>
	NODISCARD static Matrix<T, 4> CreateTransformationMatrix(const Vector3<T> &translation, const Vector3<T> &rotation,
	                                                      const Vector3<T> &scale)
	{
		Matrix<T, 4> mat; // Identity matrix
		// mat = translate(mat, translation);
		// mat = rotate(mat, glm::radians(rotation.X), glm::vec3(1, 0, 0));
		// mat = rotate(mat, glm::radians(rotation.Y), glm::vec3(0, 1, 0));
		// mat = rotate(mat, glm::radians(rotation.Z), glm::vec3(0, 0, 1));
		// mat = glm::scale(mat, scale);
		return mat;
	}

	NODISCARD FORCEINLINE static f32 LerpSmooth(f32 a, f32 b, f32 r, f32 delta)
	{
		return (a - b) * pow(r, delta) + b;
	}

	NODISCARD FORCEINLINE static Vector2F LerpSmooth(Vector2F a, Vector2F b, f32 r, f32 delta)
	{
		return {LerpSmooth(a.X, b.X, r, delta), LerpSmooth(a.Y, b.Y, r, delta)};
	}

	NODISCARD FORCEINLINE static Vector3F LerpSmooth(const Vector3F &a, const Vector3F &b, f32 r, f32 delta)
	{
		return {LerpSmooth(a.X, b.X, r, delta), LerpSmooth(a.Y, b.Y, r, delta), LerpSmooth(a.Z, b.Z, r, delta)};
	}
};

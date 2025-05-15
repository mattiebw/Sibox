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
using RectI = TRect<s32>;

class MathUtil
{
public:
	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static T DegreesToRadians(T degrees)
	{
		return static_cast<T>(static_cast<double>(degrees) * 0.017453292519943295769236907684886);
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static T RadiansToDegrees(T radians)
	{
		return static_cast<T>(static_cast<double>(radians) * 57.295779513082320876798154814105);
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	NODISCARD static T AngleRadiansFromVector(Vector2<T> vector)
	{
		return std::atan2(vector.Y, vector.X);
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	NODISCARD static T AngleDegreesFromVector(Vector2<T> vector)
	{
		return RadiansToDegrees(AngleRadiansFromVector(vector));
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	NODISCARD static Vector2<T> VectorFromAngleRadians(T angle)
	{
		return {std::cos(angle), std::sin(angle)};
	}

	template <typename T>
		requires std::is_floating_point_v<T>
	NODISCARD static Vector2<T> VectorFromAngleDegrees(T angle)
	{
		return VectorFromAngleRadians(DegreesToRadians(angle));
	}

	template <typename T>
	NODISCARD static Matrix<T, 4> CreateTransformationMatrix(const Vector3<T> &translation, const Rotator<T> &rotation,
	                                                         const Vector3<T> &scale)
	{
		Matrix<T, 4> mat; // Identity matrix
		// TRS:
		mat.Scale(scale);
		mat.Rotate(rotation);
		mat.Translate(translation);
		return mat;
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static T LerpSmooth(const T a, const T b, const T r, const T delta)
	{
		return (a - b) * pow(r, delta) + b;
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static Vector2<T> LerpSmooth(const Vector2<T> a, const Vector2<T> b, const T r, const T delta)
	{
		return {LerpSmooth(a.X, b.X, r, delta), LerpSmooth(a.Y, b.Y, r, delta)};
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static Vector2<T> LerpSmooth(const Vector2<T> &a, const Vector2<T> &b, const T r,
	                                                   const T           delta)
	{
		return {LerpSmooth(a.X, b.X, r, delta), LerpSmooth(a.Y, b.Y, r, delta), LerpSmooth(a.Z, b.Z, r, delta)};
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static T Max(T a, T b)
	{
		return a > b ? a : b;
	}
	
	template <typename T>
		requires std::is_arithmetic_v<T>
	NODISCARD FORCEINLINE static T Clamp(const T value, const T min, const T max)
	{
		return Max(min, Min(max, value));
	}

	// Fun fact: 38 digits of pi is enough to calculate the circumference of the observable universe to within the
	// width of a hydrogen atom, and 62 digits can calculate the circumference of the observable universe to within the
	// planck length (1.6162x10e-35 meters), the shortest unit of length expected to be measurable.
	// So we'll use 62 digits.
	const f64 PI = 3.14159265358979323846264338327950288419716939937510582097494459;
};

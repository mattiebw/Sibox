#pragma once

template <typename T>
	requires std::is_arithmetic_v<T>
struct Quaternion
{
	Quaternion()
		: X(0), Y(0), Z(0), W(1)
	{
	}

	Quaternion(T x, T y, T z, T w)
		: X(x), Y(y), Z(z), W(w)
	{
	}

	Quaternion(const Vector3<T> &axis, const float angleRadians)
	{
		const float halfAngleRadians = angleRadians / 2;

		W = cosf(halfAngleRadians);

		const float halfAngleSine = sinf(halfAngleRadians);
		auto        normalized    = axis.Normalized();
		X                         = normalized.X * halfAngleSine;
		Y                         = normalized.Y * halfAngleSine;
		Z                         = normalized.Z * halfAngleSine;
	}

	inline Quaternion& operator=(const Quaternion &other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
		W = other.W;
		return *this;
	}

	inline Quaternion& operator *=(const Quaternion &other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		W *= other.W;
		return *this;
	}

	inline Quaternion& operator *=(const T scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		W *= scalar;
		return *this;
	}

	inline Quaternion operator *(const Quaternion &other) const
	{
		Quaternion result;
		result.W = (W * other.W) - (X * other.X) - (Y * other.Y) - (Z * other.Z);
		result.X = (X * other.W) + (W * other.X) + (Y * other.Z) - (Z * other.Y);
		result.Y = (Y * other.W) + (W * other.Y) + (Z * other.X) - (X * other.Z);
		result.Z = (Z * other.W) + (W * other.Z) + (X * other.Y) - (Y * other.X);
		return result;
	}

	inline T MagnitudeSquared() const
	{
		return (X * X) + (Y * Y) + (Z * Z) + (W * W);
	}

	inline T Magnitude() const
	{
		return sqrtf(MagnitudeSquared());
	}

	inline void Normalize()
	{
		T invMag = 1.0f / Magnitude();

		if (0.0f * invMag == 0.0f * invMag) // Protect against NaNs
			*this *= invMag;
	}

	inline Quaternion Normalized() const
	{
		Quaternion result = *this;
		result.Normalize();
		return result;
	}

	inline void Invert()
	{
		*this *= 1.0f / MagnitudeSquared();
		X = -X;
		Y = -Y;
		Z = -Z;
	}

	inline Quaternion Inverted() const
	{
		Quaternion result = *this;
		result.Invert();
		return result;
	}

	inline Vector3<T> RotatePoint(const T x, const T y, const T z) const
	{
		Quaternion vector(x, y, z, 0);
		Quaternion final = *this * vector * Inverted();
		return Vector3<T>(final.X, final.Y, final.Z);
	}
	
	inline Vector3<T> RotatePoint(const Vector3<T> &rhs) const
	{
		Quaternion vector(rhs.X, rhs.Y, rhs.Z, 0);
		Quaternion final = *this * vector * Inverted();
		return Vector3<T>(final.X, final.Y, final.Z);
	}

	// Check for NaNs
	inline bool IsValid() const
	{
		if (X * 0.0f != X * 0.0f)
			return false;
		if (Y * 0.0f != Y * 0.0f)
			return false;
		if (Z * 0.0f != Z * 0.0f)
			return false;
		if (W * 0.0f != W * 0.0f)
			return false;

		return true;
	}

	inline Matrix<T, 3> RotateMatrix(const Matrix<T, 3> &other) const
	{
		Matrix<T, 3> result;
		// MW @todo
		return result;
	}

	inline Matrix<T, 3> ToMat3() const
	{
		Matrix<T, 3> result;

		Vector3F row1 = RotatePoint(result.Get(0, 0), result.Get(0, 1), result.Get(0, 2));
		Vector3F row2 = RotatePoint(result.Get(1, 0), result.Get(1, 1), result.Get(1, 2));
		Vector3F row3 = RotatePoint(result.Get(2, 0), result.Get(2, 1), result.Get(2, 2));

		result.Set(0, 0, row1.X);
		result.Set(0, 1, row1.Y);
		result.Set(0, 2, row1.Z);
		result.Set(1, 0, row2.X);
		result.Set(1, 1, row2.Y);
		result.Set(1, 2, row2.Z);
		result.Set(2, 0, row3.X);
		result.Set(2, 1, row3.Y);
		result.Set(2, 2, row3.Z);
		
		return result;
	}

	// Based on glm
	inline Rotator<T> EulerAngles() const
	{
		Rotator<T> result;

		T const pitchY = static_cast<T>(2) * (Y * Z + W * X);
		T const pitchX = W * W - X * X - Y * Y + Z * Z;
		if (abs(pitchX) < 0.0001f && abs(pitchY) < 0.0001f) // avoid singularities
			result.Pitch = static_cast<T>(2) * atan2f(X, W);
		else
			result.Pitch = atan2f(pitchY, pitchX);
		
		result.Yaw = asin(MathUtil::Clamp(static_cast<T>(-2) * (X * Z - W * Y), static_cast<T>(-1), static_cast<T>(1)));
		
		T const rollY = static_cast<T>(2) * (X * Y + W * Z);
		T const rollX = W * W + X * X - Y * Y - Z * Z;
		if (abs(rollX) < 0.0001f && abs(rollY) < 0.0001f) // avoid singularities
			result.Roll = 0;
		else
			result.Roll = atan2f(rollY, rollX);

		result.Pitch = MathUtil::RadiansToDegrees(result.Pitch);
		result.Yaw = MathUtil::RadiansToDegrees(result.Yaw);
		result.Roll = MathUtil::RadiansToDegrees(result.Roll);
		
		return result;
	}

	T X, Y, Z, W;
};

using QuaternionF = Quaternion<f32>;
using QuaternionD = Quaternion<f64>;
using QuaternionI = Quaternion<s32>;

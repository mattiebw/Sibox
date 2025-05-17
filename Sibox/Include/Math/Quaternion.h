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

	inline Vector3<T> RotatePoint(const Vector3<T> &rhs)
	{
		Quaternion vector(rhs.x, rhs.y, rhs.z, 0);
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
		// MW @todo
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
		
		result.Yaw   = MathUtil::RadiansToDegrees(
			asin(MathUtil::Clamp(static_cast<T>(-2) * (X * Z - W * Y), static_cast<T>(-1), static_cast<T>(1))));
		
		T const rollY = static_cast<T>(2) * (X * Y + W * Z);
		T const rollX = W * W + X * X - Y * Y - Z * Z;
		if (abs(rollX) < 0.0001f && abs(rollY) < 0.0001f) // avoid singularities
			result.Roll = 0;
		else
			result.Roll = atan2f(rollY, rollX);
		
		return result;
	}

	T X, Y, Z, W;
};

using QuaternionF = Quaternion<f32>;
using QuaternionD = Quaternion<f64>;
using QuaternionI = Quaternion<s32>;

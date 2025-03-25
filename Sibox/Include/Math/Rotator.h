#pragma once

template <typename T>
	requires std::is_arithmetic_v<T>
struct Rotator
{
	// Pitch (degrees) - On a ship or aircraft, the pitch is the rotation around the lateral axis through the center of gravity.
	T Pitch;
	// Yaw (degrees) - On a ship or aircraft, the yaw is the rotation around the vertical axis through the center of gravity.
	T Yaw;
	// Roll (degrees) - On a ship or aircraft, the roll is the rotation around the front-to-back axis through the center of gravity.
	T Roll;

	// Create a zero rotation.
	Rotator()
		: Pitch(0), Yaw(0), Roll(0)
	{
	}

	// Create a rotation with the same pitch, yaw, and roll.
	Rotator(T scalar)
		: Pitch(scalar), Yaw(scalar), Roll(scalar)
	{
	}

	// Create a rotation with the given pitch, yaw, and roll.
	Rotator(T pitch, T yaw, T roll)
		: Pitch(pitch), Yaw(yaw), Roll(roll)
	{
	}

	// Create a rotation from a vector.
	template <typename IT>
	Rotator(Vector3<IT> vec)
		: Pitch(static_cast<T>(vec.X)), Yaw(static_cast<T>(vec.Y)), Roll(static_cast<T>(vec.Z))
	{
	}

	// Get the pitch in degrees.
	NODISCARD FORCEINLINE T GetPitchDegrees() const
	{
		return Pitch;
	}

	// Get the yaw in degrees.
	NODISCARD FORCEINLINE T GetYawDegrees() const
	{
		return Yaw;
	}

	// Get the roll in degrees.
	NODISCARD FORCEINLINE T GetRollDegrees() const
	{
		return Roll;
	}

	// Get the pitch in radians.
	NODISCARD FORCEINLINE T GetPitchRadians() const
	{
		return MathUtil::DegreesToRadians(Pitch);
	}

	// Get the yaw in radians.
	NODISCARD FORCEINLINE T GetYawRadians() const
	{
		return MathUtil::DegreesToRadians(Yaw);
	}

	// Get the roll in radians.
	NODISCARD FORCEINLINE T GetRollRadians() const
	{
		return MathUtil::DegreesToRadians(Roll);
	}

	// Get the direction vector of the rotation; a unit vector pointing in the direction of the rotation, disregarding roll.
	NODISCARD Vector3<T> GetDirectionVector() const
	{
		return {cos(Yaw) * cos(Pitch), sin(Pitch), sin(Yaw) * cos(Pitch)};
	}
};

using RotatorF = Rotator<f32>;
using RotatorD = Rotator<f64>;
using RotatorI = Rotator<s32>;

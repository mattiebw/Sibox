#pragma once

#include "Core/Utility/MathUtil.h"
#include "Math/Rotator.h"

struct Transform
{
	FORCEINLINE void Translate(Vector3F translation) { Position += translation; }
	FORCEINLINE void Rotate(RotatorF rotation) { Rotation += rotation; }
	FORCEINLINE void AddScale(Vector3F scale) { Scale += scale; }
	FORCEINLINE void ScaleBy(Vector3F scale) { Scale *= scale; }

	FORCEINLINE Matrix4x4F GetTransformationMatrix() const
	{
		return MathUtil::CreateTransformationMatrix(Position, Rotation, Scale);
	}

	FORCEINLINE Vector3F GetForwardVector() const
	{
		return Matrix4x4F::MakeRotationMatrix(
			MathUtil::DegreesToRadians(Rotation.Yaw),
			MathUtil::DegreesToRadians(Rotation.Pitch),
			MathUtil::DegreesToRadians(Rotation.Roll)
			) * Vector3F::Forward();
	}
	
	FORCEINLINE Vector3F GetRightVector() const
	{
		return Matrix4x4F::MakeRotationMatrix(
			MathUtil::DegreesToRadians(Rotation.Yaw),
			MathUtil::DegreesToRadians(Rotation.Pitch),
			MathUtil::DegreesToRadians(Rotation.Roll)
			) * Vector3F::Right();
	}

	Transform() = default;

	Transform(const Vector3F &position)
		: Position(position)
	{
	}

	Transform(const Vector3F &position, const Vector3F &rotation, const Vector3F &scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	}

	Transform(const Transform &other)     = default;
	Transform(Transform &&other) noexcept = default;
	~Transform()                          = default;

	FORCEINLINE Transform& operator=(const Transform &other)     = default;
	FORCEINLINE Transform& operator=(Transform &&other) noexcept = default;

	Vector3F Position = Vector3F(0.0f);
	RotatorF Rotation = Vector3F(0.0f);
	Vector3F Scale    = Vector3F(1.0f);
};

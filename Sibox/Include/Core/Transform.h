#pragma once

#include "Core/Utility/MathUtil.h"

struct Transform
{
	FORCEINLINE void Translate(Vector3f translation) { Position += translation; }
	FORCEINLINE void Rotate(Vector3f rotation) { Rotation += rotation; }
	FORCEINLINE void AddScale(Vector3f scale) { Scale += scale; }
	FORCEINLINE void ScaleBy(Vector3f scale) { Scale *= scale; }

	FORCEINLINE glm::mat4 GetTransformationMatrix() const
	{
		return MathUtil::CreateTransformationMatrix(Position, Rotation, Scale);
	}

	Transform() = default;

	Transform(const Vector3f &position)
		: Position(position)
	{
	}

	Transform(const Vector3f &position, const Vector3f &rotation, const Vector3f &scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	}

	Transform(const Transform &other)     = default;
	Transform(Transform &&other) noexcept = default;
	~Transform()                          = default;

	FORCEINLINE Transform& operator=(const Transform &other)     = default;
	FORCEINLINE Transform& operator=(Transform &&other) noexcept = default;

	Vector3f Position = Vector3f(0.0f);
	Vector3f Rotation = Vector3f(0.0f);
	Vector3f Scale    = Vector3f(1.0f);
};

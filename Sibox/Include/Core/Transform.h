#pragma once

#include "Core/Utility/MathUtil.h"

struct Transform
{
	FORCEINLINE void Translate(glm::vec3 translation) { Position += translation; }
	FORCEINLINE void Rotate(glm::vec3 rotation) { Rotation += rotation; }
	FORCEINLINE void AddScale(glm::vec3 scale) { Scale += scale; }
	FORCEINLINE void ScaleBy(glm::vec3 scale) { Scale *= scale; }

	FORCEINLINE glm::mat4 GetTransformationMatrix() const
	{
		return MathUtil::CreateTransformationMatrix(Position, Rotation, Scale);
	}

	Transform() = default;

	Transform(const glm::vec3 &position)
		: Position(position)
	{
	}

	Transform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	}

	Transform(const Transform &other)     = default;
	Transform(Transform &&other) noexcept = default;
	~Transform()                          = default;

	FORCEINLINE Transform& operator=(const Transform &other)     = default;
	FORCEINLINE Transform& operator=(Transform &&other) noexcept = default;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale    = glm::vec3(1.0f);
};

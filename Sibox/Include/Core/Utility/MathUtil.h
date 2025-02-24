#pragma once

template <typename T = float>
	requires std::is_arithmetic_v<T>
struct TRect
{
	glm::vec<2, T> Position;
	glm::vec<2, T> Size;

	TRect(glm::vec<2, T> position, glm::vec<2, T> size)
		: Position(position), Size(size)
	{
	}

	TRect(T x, T y, T width, T height)
		: Position(glm::vec<2, T>(x, y)), Size(glm::vec<2, T>(width, height))
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

	NODISCARD FORCEINLINE bool ContainsPoint(const glm::vec2 &point) const
	{
		return Position.x <= point.x && Position.x + Size.x >= point.x
			&& Position.y <= point.y && Position.y + Size.y >= point.y;
	}

	NODISCARD FORCEINLINE glm::vec<2, T> GetCenter() const
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
	NODISCARD static glm::mat4 CreateTransformationMatrix(const glm::vec3 &translation, const glm::vec3 &rotation,
	                                                      const glm::vec3 &scale);

	NODISCARD FORCEINLINE static float LerpSmooth(float a, float b, float r, float delta)
	{
		return (a - b) * glm::pow(r, delta) + b;
	}

	NODISCARD FORCEINLINE static glm::vec2 LerpSmooth(glm::vec2 a, glm::vec2 b, float r, float delta)
	{
		return {LerpSmooth(a.x, b.x, r, delta), LerpSmooth(a.y, b.y, r, delta)};
	}

	NODISCARD FORCEINLINE static glm::vec3 LerpSmooth(const glm::vec3 &a, const glm::vec3 &b, float r, float delta)
	{
		return {LerpSmooth(a.x, b.x, r, delta), LerpSmooth(a.y, b.y, r, delta), LerpSmooth(a.z, b.z, r, delta)};
	}
};

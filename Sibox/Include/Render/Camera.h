#pragma once

class Camera
{
public:
	Transform Transformation = Transform();
	float     FOV            = 90;
	float     OrthoSize      = 24.0f;
	float     NearPlane      = 0.1f;
	float     FarPlane       = 1000.0f;
	float     Aspect         = 0;

	// MW @todo: Cache these matrices?

	NODISCARD FORCEINLINE glm::mat4 GetViewMatrix() const
	{
		return inverse(
			MathUtil::CreateTransformationMatrix(Transformation.Position, Transformation.Rotation,
			                                     Transformation.Scale));
	}

	NODISCARD FORCEINLINE glm::mat4 GetPerspectiveViewProjMatrix() const
	{
		return glm::perspective(FOV, Aspect, NearPlane, FarPlane) * GetViewMatrix();
	}

	NODISCARD FORCEINLINE glm::mat4 GetOrthographicViewProjMatrix() const
	{
		return glm::ortho(OrthoSize * Aspect / -2, OrthoSize * Aspect / 2, OrthoSize / -2, OrthoSize / 2, NearPlane,
		                  FarPlane) * GetViewMatrix();
	}

	NODISCARD FORCEINLINE FRect GetCameraRect() const
	{
		float height = OrthoSize;
		float width = height * Aspect;
		return FRect(Transformation.Position.x - width / 2, Transformation.Position.y - height / 2, width, height);
	}
	
	NODISCARD FORCEINLINE bool RectOverlapsCamera(const FRect& rect) const
	{
		return rect.OverlapsWith(GetCameraRect());
	}
};

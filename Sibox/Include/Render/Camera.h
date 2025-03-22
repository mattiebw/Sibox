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

	NODISCARD FORCEINLINE Matrix4x4F GetViewMatrix() const
	{
		// return
		// 	MathUtil::CreateTransformationMatrix(Transformation.Position, Transformation.Rotation,
		// 	                                     Transformation.Scale).Inverse();
		return Matrix4x4F(1);
	}

	NODISCARD FORCEINLINE Matrix4x4F GetPerspectiveViewProjMatrix() const
	{
		// return Matrix4x4F::MakePerspective(FOV, Aspect, NearPlane, FarPlane) * GetViewMatrix();
		return Matrix4x4F(1);
	}

	NODISCARD FORCEINLINE Matrix4x4F GetOrthographicViewProjMatrix() const
	{
		// return Matrix4x4F::MakeOrthographic(OrthoSize * Aspect / -2, OrthoSize * Aspect / 2, OrthoSize / -2, OrthoSize / 2, NearPlane,
		//                   FarPlane) * GetViewMatrix();
		return Matrix4x4F(1);
	}

	NODISCARD FORCEINLINE RectF GetCameraRect() const
	{
		float height = OrthoSize;
		float width  = height * Aspect;
		return RectF(Transformation.Position.X - width / 2, Transformation.Position.Y - height / 2, width, height);
	}

	NODISCARD FORCEINLINE bool RectOverlapsCamera(const RectF &rect) const
	{
		return rect.OverlapsWith(GetCameraRect());
	}
};

#pragma once

class Camera
{
public:
	Transform Transformation = Transform();
	f32       FOV            = 90;
	f32       OrthoSize      = 24.0f;
	f32       NearPlane      = 0.1f;
	f32       FarPlane       = 1000.0f;
	f32       Aspect         = 0;

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
		f32 height = OrthoSize;
		f32 width  = height * Aspect;
		return RectF(Transformation.Position.X - width / 2, Transformation.Position.Y - height / 2, width, height);
	}

	NODISCARD FORCEINLINE bool RectOverlapsCamera(const RectF &rect) const
	{
		return rect.OverlapsWith(GetCameraRect());
	}
};

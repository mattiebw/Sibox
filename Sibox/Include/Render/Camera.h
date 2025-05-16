#pragma once

enum class CameraMode : u8
{
	Perspective,
	Orthographic
};

inline const char* CameraModeToString(CameraMode mode)
{
	switch (mode)
	{
	case CameraMode::Perspective: return "Perspective";
	case CameraMode::Orthographic: return "Orthographic";
	default: return "Unknown";
	}
}

class Camera
{
public:
	Transform  Transformation = Transform();
	f32        FOVDegrees     = 90;
	f32        OrthoSize      = 24.0f;
	f32        NearPlane      = 0.1f;
	f32        FarPlane       = 1000.0f;
	f32        Aspect         = 0;
	CameraMode Mode           = CameraMode::Orthographic;

	// MW @todo: Cache these matrices?

	NODISCARD FORCEINLINE Matrix4x4F GetViewMatrix() const
	{
		auto tf = MathUtil::CreateTransformationMatrix(Transformation.Position, Transformation.Rotation,
		                                               Transformation.Scale);
		auto inv = tf.Inverse();
		return inv;
	}

	NODISCARD FORCEINLINE Matrix4x4F GetProjectionMatrix() const
	{
		switch (Mode)
		{
		case CameraMode::Perspective:
			return Matrix4x4F::MakePerspective(MathUtil::DegreesToRadians(FOVDegrees), Aspect, NearPlane, FarPlane);
		case CameraMode::Orthographic:
			return Matrix4x4F::MakeOrthographic(OrthoSize * Aspect / -2, OrthoSize * Aspect / 2, OrthoSize / -2,
												  OrthoSize / 2, NearPlane,
												  FarPlane);
		}

		SIBOX_ASSERT(false && "Invalid camera mode");
		return Matrix4x4F(-1);
	}

	NODISCARD FORCEINLINE Matrix4x4F GetPerspectiveViewProjMatrix() const
	{
		return Matrix4x4F::MakePerspective(MathUtil::DegreesToRadians(FOVDegrees), Aspect, NearPlane, FarPlane) * GetViewMatrix();
	}

	NODISCARD FORCEINLINE Matrix4x4F GetOrthographicViewProjMatrix() const
	{
		auto ortho = Matrix4x4F::MakeOrthographic(OrthoSize * Aspect / -2, OrthoSize * Aspect / 2, OrthoSize / -2,
		                                          OrthoSize / 2, NearPlane,
		                                          FarPlane);
		auto view = GetViewMatrix();
		return ortho * view;
	}

	NODISCARD FORCEINLINE Matrix4x4F GetViewProjectionMatrix() const
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}
	
	NODISCARD FORCEINLINE Matrix4x4F GetViewProjectionMatrixNoTranslation() const
	{
		auto tf = MathUtil::CreateTransformationMatrix(Vector3F(), Transformation.Rotation,
													   Transformation.Scale);
		auto inv = tf.Inverse();
		return GetProjectionMatrix() * inv;
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

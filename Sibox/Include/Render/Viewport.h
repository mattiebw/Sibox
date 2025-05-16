#pragma once

class World;
class Camera;
class Window;

// Viewports contain the camera, the world to render, and their width and height and offset in the window.
// They also contain the UI elements that are drawn on top of the scene.

class Viewport
{
public:
	Viewport() = default;

	void Clear() const;
	void Render() const;

	NODISCARD FORCEINLINE const Ref<Camera>& GetCamera() const { return m_Camera; }
	NODISCARD FORCEINLINE const Ref<World>&  GetWorld() const { return m_World; }
	NODISCARD FORCEINLINE Vector2I           GetOffset() const { return m_Offset; }
	NODISCARD FORCEINLINE Vector2I           GetSize() const { return m_Size; }
	NODISCARD FORCEINLINE int                GetWidth() const { return m_Size.X; }
	NODISCARD FORCEINLINE int                GetHeight() const { return m_Size.Y; }

	FORCEINLINE void SetWorld(const Ref<World> &world) { m_World = world; }
	void             SetCamera(const Ref<Camera> &camera);
	FORCEINLINE void SetOffset(const Vector2I &offset) { m_Offset = offset; }
	void             SetSize(const Vector2I &size);
	FORCEINLINE void SetOffsetAndSize(const Vector2I &offset, const Vector2I &size)
	{
		m_Offset = offset;
		SetSize(size);
	}

	bool     ShouldClear = true;
	Vector4F ClearColor  = {0.1f, 0.1f, 0.1f, 1};

protected:
	Ref<World>  m_World;
	Ref<Camera> m_Camera;
	Vector2I    m_Offset;
	Vector2I    m_Size;
};

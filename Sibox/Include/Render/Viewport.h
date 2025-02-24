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

	void Render();

	NODISCARD FORCEINLINE const Ref<Camera>& GetCamera() const { return m_Camera; }
	NODISCARD FORCEINLINE const Ref<World>&  GetWorld() const { return m_World; }
	NODISCARD FORCEINLINE glm::ivec2         GetOffset() const { return m_Offset; }
	NODISCARD FORCEINLINE glm::ivec2         GetSize() const { return m_Size; }
	NODISCARD FORCEINLINE int                GetWidth() const { return m_Size.x; }
	NODISCARD FORCEINLINE int                GetHeight() const { return m_Size.y; }

	FORCEINLINE void SetWorld(const Ref<World> &world) { m_World = world; }
	void             SetCamera(const Ref<Camera> &camera);
	FORCEINLINE void SetOffset(const glm::ivec2 &offset) { m_Offset = offset; }
	void             SetSize(const glm::ivec2 &size);
	FORCEINLINE void SetOffsetAndSize(const glm::ivec2 &offset, const glm::ivec2 &size)
	{
		m_Offset = offset;
		SetSize(size);
	}

	bool      ShouldClear = true;
	glm::vec4 ClearColor  = {0.1f, 0.1f, 0.1f, 1};

protected:
	Ref<World>  m_World;
	Ref<Camera> m_Camera;
	glm::ivec2  m_Offset;
	glm::ivec2  m_Size;
};

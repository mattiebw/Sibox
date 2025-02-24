#include "papipch.h"
#include "Render/Viewport.h"

#include "Render/Camera.h"
#include "World/World.h"

void Viewport::Render()
{
	glViewport(m_Offset.x, m_Offset.y, m_Size.x, m_Size.y);
	if (ShouldClear)
	{
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (m_World)
		m_World->Render();
}

void Viewport::SetCamera(const Ref<Camera> &camera)
{
	m_Camera = camera;
	if (m_Camera)
		m_Camera->Aspect = static_cast<float>(m_Size.x) / static_cast<float>(m_Size.y);
}

void Viewport::SetSize(const glm::ivec2 &size)
{
	if (m_Camera)
		m_Camera->Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
	m_Size = size;
}

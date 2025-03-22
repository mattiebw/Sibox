#include "siboxpch.h"
#include "Render/Viewport.h"

#include "Render/Camera.h"
#include "World/World.h"

void Viewport::Render()
{
	glViewport(m_Offset.X, m_Offset.Y, m_Size.X, m_Size.Y);
	if (ShouldClear)
	{
		glClearColor(ClearColor.R, ClearColor.G, ClearColor.B, ClearColor.A);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (m_World)
		m_World->Render();
}

void Viewport::SetCamera(const Ref<Camera> &camera)
{
	m_Camera = camera;
	if (m_Camera)
		m_Camera->Aspect = static_cast<f32>(m_Size.X) / static_cast<f32>(m_Size.Y);
}

void Viewport::SetSize(const Vector2I &size)
{
	if (m_Camera)
		m_Camera->Aspect = static_cast<f32>(size.X) / static_cast<f32>(size.Y);
	m_Size = size;
}

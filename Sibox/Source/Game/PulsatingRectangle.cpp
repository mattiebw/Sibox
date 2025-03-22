#include "siboxpch.h"
#include "Game/PulsatingRectangle.h"

#include "Core/Application.h"
#include "Core/Random.h"
#include "Render/Renderer.h"

void PulsatingRectangle::Created()
{
	m_Color = Vector3F(Random::Float(), Random::Float(), Random::Float());
	SetPosition(Vector3F(Random::Float(-10.0f, 10.0f), Random::Float(-10.0f, 10.0f), -0.05f));
	m_Time += Random::Float();
}

void PulsatingRectangle::Tick(double delta)
{
	m_Time += static_cast<float>(delta);
}

void PulsatingRectangle::Render()
{
	float sine = sin(m_Time * 5);
	Application::GetRenderer()->GetQuadRenderer()->DrawQuad(GetPosition(), Vector2F(1.0f + sine, 1.0f),
	                                                        Vector4F(m_Color, 1.0f));
}

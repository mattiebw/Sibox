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

void PulsatingRectangle::Tick(f64 delta)
{
	m_Time += static_cast<f32>(delta);
	f32 sine = sin(m_Time * 5);
	EntityTransform.Scale = Vector3F(sine, sine / 2, 1);
	// EntityTransform.Rotation = Rotator(0.f, 0.f, sine * 360.f);
}

void PulsatingRectangle::Render()
{
	Application::GetRenderer()->GetQuadRenderer()->DrawQuad(EntityTransform.GetTransformationMatrix(),
	                                                        Vector4F(m_Color, 1.0f));
}

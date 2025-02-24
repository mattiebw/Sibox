#include "papipch.h"
#include "Game/PulsatingRectangle.h"

#include "Core/Application.h"
#include "Core/Random.h"
#include "Render/Renderer.h"

void PulsatingRectangle::Created()
{
	m_Color = glm::vec3(Random::Float(), Random::Float(), Random::Float());
	SetPosition(glm::vec3(Random::Float(-10.0f, 10.0f), Random::Float(-10.0f, 10.0f), -0.05f));
	m_Time += Random::Float();
}

void PulsatingRectangle::Tick(double delta)
{
	m_Time += static_cast<float>(delta);
}

void PulsatingRectangle::Render()
{
	float sine = glm::sin(m_Time * 5);
	Application::GetRenderer()->GetQuadRenderer()->DrawQuad(GetPosition(), glm::vec2(1.0f + sine, 1.0f),
	                                                        glm::vec4(m_Color, 1.0f));
}

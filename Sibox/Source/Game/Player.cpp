#include "siboxpch.h"
#include "Game/Player.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Game/PulsatingRectangle.h"
#include "World/World.h"
#include "Render/Camera.h"
#include "Render/Font.h"
#include "Render/Renderer.h"
#include "Render/Viewport.h"
#include "Audio/AudioManager.h"

namespace {
	static SoundHandle footstepHandle;
	static bool wasMoving = false;
}

void Player::OnPersonaNameChange(PersonaStateChange_t *parameter)
{
	Name = SteamFriends()->GetPersonaName();
}

void Player::Created()
{
	m_Camera                            = CreateRef<Camera>();
	m_Camera->Transformation.Position.z = 3;

	if (Application::Get()->HasFrontend())
	{
		m_Texture = CreateRef<Texture>("Content/Textures/jeremy.jpeg");

		m_Viewport = Application::GetRenderer()->CreateViewport();
		m_Viewport->SetCamera(m_Camera);
	}

	Name = SteamFriends()->GetPersonaName();
}

void Player::AddedToWorld(World *world)
{
	if (Application::Get()->HasFrontend())
	{
		m_Viewport->SetWorld(Application::Get()->GetWorldFromPointer(m_World));
	}

	// for (int i = 0; i < 10; i++)
	// 	world->AddEntity<PulsatingRectangle>();
}

void Player::Tick(double delta)
{
	m_Time += static_cast<float>(delta);

	switch (m_EntityNetworkType)
	{
		case EntityNetworkType::LocalOnly:
		case EntityNetworkType::RemoteOwned:
			{
				glm::vec2 input(0.0f);
				input.x -= Input::IsKeyDown(SIBOX_KEY_A) ? 1.0f : 0.0f;
				input.x += Input::IsKeyDown(SIBOX_KEY_D) ? 1.0f : 0.0f;
				input.y -= Input::IsKeyDown(SIBOX_KEY_S) ? 1.0f : 0.0f;
				input.y += Input::IsKeyDown(SIBOX_KEY_W) ? 1.0f : 0.0f;

				bool isMoving = (input.x != 0.0f || input.y != 0.0f);

				if (!wasMoving && isMoving)
				{
					footstepHandle = AudioManager::PlaySound("event:/SFX/Footsteps");
					footstepHandle.SetVolume(2.0f);
				}

				if (wasMoving && !isMoving)
				{
					footstepHandle.Pause(true);
				}

				wasMoving = isMoving;

				if (input.x != 0 || input.y != 0)
				{
					input = normalize(input);
					FRect collision(EntityTransform.Position.x - 0.45f, EntityTransform.Position.y - 0.45f, 0.9f, 0.9f);
					collision.Position.x += input.x * static_cast<float>(delta) * 5.0f;
					if (m_World->RectOverlapsAnySolidTile(collision))
						collision.Position.x = EntityTransform.Position.x - 0.45f;
					collision.Position.y += input.y * static_cast<float>(delta) * 5.0f;
					if (m_World->RectOverlapsAnySolidTile(collision))
						collision.Position.y = EntityTransform.Position.y - 0.45f;
					EntityTransform.Position = glm::vec3(collision.Position.x + 0.45f, collision.Position.y + 0.45f,
														 EntityTransform.Position.z);
				}
			}
			break;
		default:
			break;
	}
	
	bool pDown = Input::IsKeyDown(SIBOX_KEY_P);
	bool bDown = Input::IsKeyDown(SIBOX_KEY_B);
	if (pDown && !m_WasPDown)
	{
		AudioManager::PlayBackgroundMusic();
	}
	if (bDown && !m_WasBDown)
	{
		AudioManager::PlayPreviousMusic();
	}
	m_WasPDown = pDown;
	m_WasBDown = bDown;

	m_Camera->Transformation.Position.x = MathUtil::LerpSmooth(m_Camera->Transformation.Position.x,
	                                                           EntityTransform.Position.x, 0.001f,
	                                                           static_cast<float>(delta));
	m_Camera->Transformation.Position.y = MathUtil::LerpSmooth(m_Camera->Transformation.Position.y,
	                                                           EntityTransform.Position.y, 0.001f,
	                                                           static_cast<float>(delta));
}

void Player::Render()
{
	float sine = (glm::sin(m_Time) + 1) / 2;
	Application::GetQuadRenderer()->DrawQuad(EntityTransform.Position, glm::vec2(1.0f),
	                                         glm::vec4(sine, sine, sine, 1.0f), m_Texture);

	// MW @todo: optimise, create a struct to wrap drawing a string allowing us to cache the measurement
	// Stopwatch sw;
	Transform tf   = EntityTransform;
	tf.Position.y += .8f;
	tf.Scale = glm::vec3(0.5, 0.5, 1.0);
	FontMeasurement measurement = Font::GetDefaultFont()->MeasureString(Name, tf.Scale);
	tf.Position.x -= measurement.Size.x / 2;
	Application::GetTextRenderer().DrawString(Name, Font::GetDefaultFont(), tf.GetTransformationMatrix(), glm::vec4(1));
	tf.Position.x += measurement.Size.x / 2;
	tf.Position.y += measurement.Size.y / 2;
	tf.Position += glm::vec3(measurement.Offset, 0);
	tf.Scale = glm::vec3(measurement.Size + glm::vec2(0.1f, 0.1f), 1);
	tf.Position.z -= 0.05f;
	Application::GetQuadRenderer()->DrawQuad(tf.GetTransformationMatrix(), glm::vec4(1, 1, 1, .25));
	// sw.End();
	// SIBOX_TRACE("DrawString took {0}us ({1} ms)", sw.GetElapsedMicroseconds(), sw.GetElapsedMilliseconds());

	// auto rect = m_Camera->GetCameraRect();
	// rect.Position += glm::vec2(0.1f, 0.1f);
	// rect.Size -= glm::vec2(0.2f, 0.2f);
	// Application::GetQuadRenderer()->DrawQuad(glm::vec3(rect.GetCenter(), 1), rect.Size, glm::vec4(1, 0, 0, 0.5f));
}

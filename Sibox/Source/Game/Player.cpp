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

void Player::OnPersonaNameChange(PersonaStateChange_t *parameter)
{
	Name = SteamFriends()->GetPersonaName();
}

void Player::Created()
{
	m_Camera                            = CreateRef<Camera>();
	m_Camera->Transformation.Position.Z = 3;

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
			Vector2F input(0.0f);
			input.X -= Input::IsKeyDown(SIBOX_KEY_A) ? 1.0f : 0.0f;
			input.X += Input::IsKeyDown(SIBOX_KEY_D) ? 1.0f : 0.0f;
			input.Y -= Input::IsKeyDown(SIBOX_KEY_S) ? 1.0f : 0.0f;
			input.Y += Input::IsKeyDown(SIBOX_KEY_W) ? 1.0f : 0.0f;

			if (input.X != 0 || input.Y != 0)
			{
				input.Normalize();
				RectF collision(EntityTransform.Position.X - 0.45f, EntityTransform.Position.Y - 0.45f, 0.9f, 0.9f);
				collision.Position.X += input.X * static_cast<float>(delta) * 5.0f;
				if (m_World->RectOverlapsAnySolidTile(collision))
					collision.Position.X = EntityTransform.Position.X - 0.45f;
				collision.Position.Y += input.Y * static_cast<float>(delta) * 5.0f;
				if (m_World->RectOverlapsAnySolidTile(collision))
					collision.Position.Y = EntityTransform.Position.Y - 0.45f;
				EntityTransform.Position = Vector3F(collision.Position.X + 0.45f, collision.Position.Y + 0.45f,
				                                    EntityTransform.Position.Z);
			}
		}
		break;
	default:
		break;
	}

	m_Camera->Transformation.Position.X = MathUtil::LerpSmooth(m_Camera->Transformation.Position.X,
	                                                           EntityTransform.Position.X, 0.001f,
	                                                           static_cast<float>(delta));
	m_Camera->Transformation.Position.Y = MathUtil::LerpSmooth(m_Camera->Transformation.Position.Y,
	                                                           EntityTransform.Position.Y, 0.001f,
	                                                           static_cast<float>(delta));
}

void Player::Render()
{
	float sine = (sin(m_Time) + 1) / 2;
	Application::GetQuadRenderer()->DrawQuad(EntityTransform.Position, Vector2F(1.0f),
	                                         Vector4F(sine, sine, sine, 1.0f), m_Texture);

	// MW @todo: optimise, create a struct to wrap drawing a string allowing us to cache the measurement
	// Stopwatch sw;
	Transform tf = EntityTransform;
	tf.Position.Y += .8f;
	tf.Scale                    = Vector3F(0.5, 0.5, 1.0);
	FontMeasurement measurement = Font::GetDefaultFont()->MeasureString(Name, tf.Scale);
	tf.Position.X -= measurement.Size.X / 2;
	Application::GetTextRenderer().DrawString(Name, Font::GetDefaultFont(), tf.GetTransformationMatrix(), Vector4F(1));
	tf.Position.X += measurement.Size.X / 2;
	tf.Position.Y += measurement.Size.Y / 2;
	tf.Position += Vector3F(measurement.Offset, 0);
	tf.Scale = Vector3F(measurement.Size + Vector2F(0.1f, 0.1f), 1);
	tf.Position.Z -= 0.05f;
	Application::GetQuadRenderer()->DrawQuad(tf.GetTransformationMatrix(), Vector4F(1, 1, 1, .25));
	// sw.End();
	// SIBOX_TRACE("DrawString took {0}us ({1} ms)", sw.GetElapsedMicroseconds(), sw.GetElapsedMilliseconds());

	// auto rect = m_Camera->GetCameraRect();
	// rect.Position += Vector2F(0.1f, 0.1f);
	// rect.Size -= Vector2F(0.2f, 0.2f);
	// Application::GetQuadRenderer()->DrawQuad(Vector3F(rect.GetCenter(), 1), rect.Size, Vector4F(1, 0, 0, 0.5f));
}

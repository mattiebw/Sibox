#include "siboxpch.h"
#include "Game/Player.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Game/PulsatingRectangle.h"
#include "Render/Camera.h"
#include "Render/Renderer.h"
#include "Render/Viewport.h"

void Player::Created()
{
	m_Camera                            = CreateRef<Camera>();
	m_Camera->Transformation.Position.Z = 3;
	m_Camera->Mode                      = CameraMode::Perspective;

	if (Application::Get()->HasFrontend())
	{
		m_Viewport = Application::GetRenderer()->CreateViewport();
		m_Viewport->SetCamera(m_Camera);
	}
}

void Player::AddedToWorld(World *world)
{
	if (Application::Get()->HasFrontend())
	{
		m_Viewport->SetWorld(Application::Get()->GetWorldFromPointer(m_World));
	}
}

void Player::Tick(f64 delta)
{
	m_Time += static_cast<f32>(delta);

	switch (m_EntityNetworkType)
	{
	case EntityNetworkType::LocalOnly:
	case EntityNetworkType::RemoteOwned:
		{
			bool isCursorLocked = Application::Get()->IsCursorLocked();
			if (!isCursorLocked && Input::IsMouseButtonDown(SIBOX_MOUSE_BUTTON_LEFT))
			{
				Application::Get()->LockCursor();
			}
			else if (isCursorLocked && Input::IsKeyDownThisFrame(SIBOX_KEY_ESCAPE))
			{
				Application::Get()->UnlockCursor();
			}

			if (isCursorLocked)
			{
				Vector2F mouseMovement = Input::GetMouseDelta();
				EntityTransform.Rotation.Yaw += mouseMovement.X * 0.1f;
				EntityTransform.Rotation.Pitch += mouseMovement.Y * 0.1f;
				EntityTransform.Rotation.Pitch = MathUtil::Clamp(EntityTransform.Rotation.Pitch, -89.0f, 89.0f);
			}
			
			Vector2F input(0.0f);
			input.X -= Input::IsKeyDown(SIBOX_KEY_A) ? 1.0f : 0.0f;
			input.X += Input::IsKeyDown(SIBOX_KEY_D) ? 1.0f : 0.0f;
			input.Y -= Input::IsKeyDown(SIBOX_KEY_S) ? 1.0f : 0.0f;
			input.Y += Input::IsKeyDown(SIBOX_KEY_W) ? 1.0f : 0.0f;
			if (input.X != 0 || input.Y != 0)
			{
				input.Normalize();
				EntityTransform.Position += EntityTransform.GetForwardVector() * input.X * 8.0f * static_cast<f32>(
					delta);
				EntityTransform.Position += EntityTransform.GetRightVector() * input.Y * 8.0f * static_cast<f32>(delta);
			}

			if (Input::IsKeyDown(SIBOX_KEY_SPACE))
			{
				EntityTransform.Position.Y += 8.0f * static_cast<f32>(delta);
			}

			if (Input::IsKeyDown(SIBOX_KEY_LSHIFT))
			{
				EntityTransform.Position.Y -= 8.0f * static_cast<f32>(delta);
			}
		}
		break;
	default:
		break;
	}

	m_Camera->Transformation = EntityTransform;
}

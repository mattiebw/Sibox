#include "siboxpch.h"
#include "Game/SiboxLayer.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Game/Player.h"
#include "World/TileSets.h"
#include "World/World.h"

#include <misc/cpp/imgui_stdlib.h>

#include "Game/PulsatingRectangle.h"
#include "Render/Camera.h"
#include "World/ChunkProvider.h"
#include "World/TileMap.h"

void SiboxLayer::OnAttach()
{
	SIBOX_INFO("Initialising Sibox game!");

	// Bodging some stuff!
	TileSets::Init();
	Application *app   = Application::Get();
	Ref<World>   world = app->AddWorld();
	m_Player           = world->AddEntity<Player>("Player");

	for (s32 i = 0; i < 100; i++)
	{
		auto rect = world->AddEntity<PulsatingRectangle>();
		rect->EntityTransform.Position = Vector3F(Random::Float(-10, 10), Random::Float(-10, 10), Random::Float(-10, 10));
	}
	
	auto tilemap = world->CreateTileMap(TileSets::MainTileSet);
	tilemap->Z   = -10;
	tilemap->SetChunkProvider(CreateRef<DefaultChunkProvider>());

	m_TextTransform.Position.Z = -0.05f;
}

void SiboxLayer::OnDetach()
{
	TileSets::Shutdown();
}

void SiboxLayer::Update(f64 delta)
{
}

void SiboxLayer::Render(f64 delta)
{
	// Application::GetTextRenderer().DrawString("Hello", Font::GetDefaultFont(),
	//                                           m_TextTransform.GetTransformationMatrix(), glm::vec4(1.0f));
}

void SiboxLayer::RenderImGUI(f64 delta)
{
	ImGui::Begin("Toybox");
	f32 ts = Application::Get()->GetTimeScale();
	ImGui::DragFloat("Timescale", &ts, 0.01f, 0.0f, 10.0f);
	if (ImGui::Button("Reset Time Scale"))
		ts = 1.0f;
	Application::Get()->SetTimeScale(ts);
	ImGui::InputText("Player Name", &m_Player->Name);
	ImGui::InputFloat2("Player Position", m_Player->EntityTransform.Position.Data());
	ImGui::DragFloat3("Player Rotation", m_Player->EntityTransform.Rotation.Data());
	if (ImGui::BeginCombo("Camera Type", CameraModeToString(m_Player->GetCamera()->Mode)))
	{
		if (ImGui::Selectable("Perspective", m_Player->GetCamera()->Mode == CameraMode::Perspective))
			m_Player->GetCamera()->Mode = CameraMode::Perspective;
		if (ImGui::Selectable("Orthographic", m_Player->GetCamera()->Mode == CameraMode::Orthographic))
			m_Player->GetCamera()->Mode = CameraMode::Orthographic;
		ImGui::EndCombo();
	}
	ImGui::DragFloat("Camera Zoom", &m_Player->GetCamera()->OrthoSize, 0.1f, 0.0f, 100.0f);
	ImGui::End();
}

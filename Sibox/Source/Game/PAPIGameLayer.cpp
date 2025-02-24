#include "papipch.h"
#include "Game/PAPIGameLayer.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Game/Player.h"
#include "World/TileSets.h"
#include "World/World.h"

#include <misc/cpp/imgui_stdlib.h>

#include "Render/Camera.h"
#include "World/ChunkProvider.h"
#include "World/TileMap.h"

void PAPIGameLayer::OnAttach()
{
	PAPI_INFO("Initialising PAPI game!");

	// Bodging some stuff!
	TileSets::Init();
	Application *app   = Application::Get();
	Ref<World>   world = app->AddWorld();
	m_Player           = world->AddEntity<Player>("Player");

	auto tilemap = world->CreateTileMap(TileSets::MainTileSet);
	tilemap->Z = -10;
	tilemap->SetChunkProvider(CreateRef<DefaultChunkProvider>());

	m_TextTransform.Position.z = -0.05f;
}

void PAPIGameLayer::OnDetach()
{
	TileSets::Shutdown();
}

void PAPIGameLayer::Update(double delta)
{
}

void PAPIGameLayer::Render(double delta)
{
	// Application::GetTextRenderer().DrawString("Hello", Font::GetDefaultFont(),
	//                                           m_TextTransform.GetTransformationMatrix(), glm::vec4(1.0f));
}

void PAPIGameLayer::RenderImGUI(double delta)
{
	ImGui::Begin("Player Info");
	ImGui::InputText("Player Name", &m_Player->Name);
	ImGui::InputFloat2("Player Position", glm::value_ptr(m_Player->EntityTransform.Position));
	ImGui::DragFloat("Camera Zoom", &m_Player->GetCamera()->OrthoSize, 0.1f, 0.0f, 100.0f);
	ImGui::End();
}

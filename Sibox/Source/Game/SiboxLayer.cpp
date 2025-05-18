#include "siboxpch.h"
#include "Game/SiboxLayer.h"

#include "Core/Application.h"
#include "Core/Input/Input.h"
#include "Game/Player.h"
#include "Game/MeshEntity.h"
#include "World/TileSets.h"
#include "World/World.h"

#include "Game/Assets.h"
#include "Physics/Entities/SphereEntity.h"
#include "Render/Camera.h"
#include "World/ChunkProvider.h"

void SiboxLayer::OnAttach()
{
	SIBOX_INFO("Initialising Sibox game!");

	// Bodging some stuff!
	Assets::Initialise();
	Application *app   = Application::Get();
	Ref<World>   world = app->AddWorld();
	m_Player           = world->AddEntity<Player>("Player");

	// {
	// 	LOG_SCOPE_TIMER("Loading mesh");
	// 	Ref<Mesh> testMesh = CreateRef<Mesh>(MeshSpecification());
	// 	testMesh->LoadFromFile("Content/Meshes/Test.mesh");
	//
	// 	m_Mesh = world->AddEntity<MeshEntity>();
	// 	m_Mesh->SetMesh(testMesh);
	// 	m_Mesh->EntityTransform.Position.Z = 10;
	// }

	for (s32 i = 0; i < 20; i++)
	{
		auto sphere = world->AddEntity<SphereEntity>("Sphere");
		sphere->Setup(Random::Float(0.5f, 2.f));
		sphere->SetPosition(Vector3F(Random::Float(-10.f, 10.f), Random::Float(-10.f, 10.f), Random::Float(-10.f, 10.f)));
	}
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
}

void SiboxLayer::RenderImGUI(f64 delta)
{
	ImGui::Begin("Toybox");

	ImGui::Text("Timescale");
	f32 ts = static_cast<f32>(Application::Get()->GetTimeScale());
	ImGui::DragFloat("Timescale", &ts, 0.01f, 0.0f, 10.0f);
	if (ImGui::Button("Reset Time Scale"))
		ts = 1.0f;
	Application::Get()->SetTimeScale(ts);
	ImGui::Dummy(ImVec2(0, 20));

	ImGui::Text("Player");
	ImGui::DragFloat3("Player Position", m_Player->EntityTransform.Position.Data());
	ImGui::DragFloat3("Player Rotation", m_Player->EntityTransform.Rotation.Data());
	ImGui::Dummy(ImVec2(0, 20));
	
	ImGui::Text("Camera");
	if (ImGui::BeginCombo("Camera Type", CameraModeToString(m_Player->GetCamera()->Mode)))
	{
		if (ImGui::Selectable("Perspective", m_Player->GetCamera()->Mode == CameraMode::Perspective))
			m_Player->GetCamera()->Mode = CameraMode::Perspective;
		if (ImGui::Selectable("Orthographic", m_Player->GetCamera()->Mode == CameraMode::Orthographic))
			m_Player->GetCamera()->Mode = CameraMode::Orthographic;
		ImGui::EndCombo();
	}
	if (m_Player->GetCamera()->Mode == CameraMode::Perspective)
	{
		ImGui::DragFloat("FOV", &m_Player->GetCamera()->FOVDegrees, 0.1f, 0.1f, 179.9f);
	}
	else
	{
		ImGui::DragFloat("Ortho Size", &m_Player->GetCamera()->OrthoSize, 0.1f, 0.1f, 1000.0f);
	}
	ImGui::Dummy(ImVec2(0, 20));

	// ImGui::Text("Mesh");
	// ImGui::DragFloat3("Mesh Position", m_Mesh->EntityTransform.Position.Data());
	// ImGui::DragFloat3("Mesh Rotation", m_Mesh->EntityTransform.Rotation.Data());
	// ImGui::DragFloat3("Mesh Scale", m_Mesh->EntityTransform.Scale.Data());
	
	ImGui::End();
}

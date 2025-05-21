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
	m_World = app->AddWorld();
	m_Player           = m_World->AddEntity<Player>("Player");
	m_Player->SetPosition({ 0, 10.0f, -100.0f });

	// {
	// 	LOG_SCOPE_TIMER("Loading mesh");
	// 	Ref<Mesh> testMesh = CreateRef<Mesh>(MeshSpecification());
	// 	testMesh->LoadFromFile("Content/Meshes/Test.mesh");
	//
	// 	m_Mesh = world->AddEntity<MeshEntity>();
	// 	m_Mesh->SetMesh(testMesh);
	// 	m_Mesh->EntityTransform.Position.Z = 10;
	// }

	// auto baseBox = m_World->AddEntity<AABBEntity>("Base");
	// baseBox->Setup(Vector3F(100, 5, 100), 0.0f);
	// baseBox->SetBodyPosition(Vector3F(0.f, -50.f, 0.0f));

	auto baseSphere = m_World->AddEntity<SphereEntity>("Base");
	baseSphere->Setup(1000.0f, 0.0f);
	baseSphere->SetBodyPosition(Vector3F(0.f, -1000.f, 0.0f));
	baseSphere->GetBody()->Elasticity = 1.0f;
}

void SiboxLayer::OnDetach()
{
	TileSets::Shutdown();
}

void SiboxLayer::Update(f64 delta)
{
	if (Input::IsMouseButtonDownThisFrame(SIBOX_MOUSE_BUTTON_RIGHT))
	{
		auto sphere = m_World->AddEntity<SphereEntity>("Sphere");
		sphere->Setup(Random::Float(0.5f, 2.f));
		Vector3F playerLookDirection = m_Player->GetCamera()->Transformation.GetRightVector();
		Vector3F playerPosition      = m_Player->EntityTransform.Position;
		Vector3F spawnPosition       = playerPosition + playerLookDirection * 5.0f;
		sphere->SetBodyPosition(spawnPosition);
		sphere->GetBody()->LinearVelocity = playerLookDirection * 100.0f;
		m_DynamicBodies.push_back(sphere.get());
	}
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
	ImGui::SliderFloat("Player Speed", &m_Player->Speed, 0.0f, 250.0f);
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

	ImGui::Text("Physics");
	ImGui::Text("Bodies: %llu", m_World->GetPhysicsScene().GetBodyCount());
	if (ImGui::Button("Clear Dynamic Bodies"))
	{
		for (Entity *entity : m_DynamicBodies)
			m_World->DestroyEntity(entity);
		m_DynamicBodies.clear();
	}
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("Sphere Height", &m_SphereHeight, 0.1f, -1000.0f, 1000.0f);
	ImGui::SliderInt("##spheresToAdd", &m_SpheresToAdd, 1, 150);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGui::Button("Add Spheres"))
		AddSpheres();
	if (ImGui::Button("Add Sphere at Origin"))
		AddSphere(Vector3F(0.0f, 0.0f, 0.0f));
	if (ImGui::Button("Add Sphere at Player"))
		AddSphere(m_Player->EntityTransform.Position);
	
	ImGui::End();
}

void SiboxLayer::AddSpheres()
{
	for (s32 i = 0; i < m_SpheresToAdd; i++)
	{
		auto sphere = m_World->AddEntity<SphereEntity>("Sphere");
		sphere->Setup(Random::Float(0.5f, 2.f));
		sphere->SetBodyPosition(Vector3F(Random::Float(-100.f, 100.f), m_SphereHeight, Random::Float(-100.f, 100.f)));
		m_DynamicBodies.push_back(sphere.get());
	}
}

void SiboxLayer::AddSphere(const Vector3F &location)
{
	auto sphere = m_World->AddEntity<SphereEntity>("Sphere");
	sphere->Setup(1.0f);
	sphere->SetBodyPosition(location);
	m_DynamicBodies.push_back(sphere.get());
}

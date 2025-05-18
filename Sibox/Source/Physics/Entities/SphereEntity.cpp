#include "siboxpch.h"
#include "Physics/Entities/SphereEntity.h"

#include "Core/Application.h"
#include "Game/Assets.h"
#include "World/World.h"

void SphereEntity::Setup(f32 radius, f32 invMass)
{
	m_Body              = new Body(Shape::CreateSphere(radius));
	m_Body->InverseMass = invMass;
	m_World->GetPhysicsScene().AddBody(m_Body);
	m_Material.Texture0 = invMass == 0 ? Assets::GreyGrid : Assets::GreenGrid;
}

void SphereEntity::Tick(f64 delta)
{
	Entity::Tick(delta);
	EntityTransform.Position = m_Body->Position;
	EntityTransform.Rotation = m_Body->Rotation.EulerAngles();
	EntityTransform.Scale    = Vector3F(m_Body->GetShape().GetRadius());
}

void SphereEntity::Render()
{
	Application::GetRenderer()->DrawMesh(
		m_Body->GetShape().GetRadius() > 200 ? Assets::VeryFuckingDetailedSphere.get() : Assets::Sphere.get(),
		EntityTransform.GetTransformationMatrix(),
		&m_Material, 1);
}

void SphereEntity::Destroyed()
{
	Entity::Destroyed();
	m_World->GetPhysicsScene().RemoveBody(m_Body);
}

void SphereEntity::SetBodyPosition(Vector3F pos)
{
	m_Body->Position         = pos;
	EntityTransform.Position = pos;
}

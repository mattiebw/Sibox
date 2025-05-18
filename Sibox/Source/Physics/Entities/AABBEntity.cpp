#include "siboxpch.h"
#include "Physics/Entities/AABBEntity.h"

#include "Core/Application.h"
#include "Game/Assets.h"
#include "Physics/Body.h"
#include "Physics/Shape.h"
#include "World/World.h"

void AABBEntity::Setup(Vector3F size, f32 invMass)
{
	m_Body = new Body(Shape::CreateAABB(size));
	m_Body->InverseMass = invMass;
	m_World->GetPhysicsScene().AddBody(m_Body);
	m_Material.Texture0 = invMass == 0 ? Assets::GreyGrid : Assets::GreenGrid;
}

void AABBEntity::Tick(f64 delta)
{
	Entity::Tick(delta);
	EntityTransform.Position = m_Body->Position;
	EntityTransform.Scale = m_Body->GetShape().GetSize();
}

void AABBEntity::Render()
{
	Application::GetRenderer()->DrawMesh(Assets::Cube.get(), EntityTransform.GetTransformationMatrix(),
										 &m_Material, 1);
}

void AABBEntity::Destroyed()
{
	Entity::Destroyed();
	m_World->GetPhysicsScene().RemoveBody(m_Body);
}

void AABBEntity::SetBodyPosition(Vector3F pos)
{
	m_Body->Position = pos;
	EntityTransform.Position = pos;
}

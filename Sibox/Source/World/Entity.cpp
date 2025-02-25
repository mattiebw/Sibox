#include "siboxpch.h"
#include "World/Entity.h"

#include "Game/Player.h"
#include "Game/PulsatingRectangle.h"
#include "World/World.h"

#define ENTITY_TYPE_CASE(typeName) case typeName::s_EntityTypeID: return CreateRef<typeName>(name, tf, uuid);

Entity::Entity()
{
}

Entity::Entity(std::string name, UUID uuid)
	: m_UUID(uuid), m_Name(std::move(name))
{
}

Entity::Entity(std::string name, const Transform &tf, UUID uuid)
	: EntityTransform(tf), m_UUID(uuid), m_Name(std::move(name))
{
}

Entity::Entity(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, UUID uuid)
	: EntityTransform(pos, rot, scale), m_UUID(uuid), m_Name(std::move(name))
{
}

Entity::~Entity()
{
}

Ref<Entity> Entity::CreateEntity(uint16_t type, std::string name, const Transform &tf, UUID uuid)
{
	// MW @copypaste: Every entity that can be created dynamically needs an entry here.
	switch (type)
	{
	ENTITY_TYPE_CASE(Entity)
	ENTITY_TYPE_CASE(Player)
	ENTITY_TYPE_CASE(PulsatingRectangle)
	default: break;
	}

	SIBOX_ERROR("Tried to create entity with missing type ID {}", type);
	return nullptr;
}

Ref<Entity> Entity::CreateEntity(uint16_t type, std::string name, UUID uuid)
{
	return CreateEntity(type, name, Transform(), uuid);
}

void Entity::SetUUID(UUID uuid)
{
	if (m_World)
		m_World->UpdateEntityUUID(m_UUID, uuid);
	m_UUID = uuid;
}

void Entity::SendEvent(uint16_t eventID, Buffer eventData)
{
	// MW @todo: Implement this!
	// We need tell the server that we're calling this, with our UUID.
	// The server will check the relevancy of this entity to all players, and send this event to those who need it.
	// We'll then receive the event on all clients, who can check the event ID in a switch statement and handle it
	// if needed.
}

void Entity::ReceiveEntityEvent(uint16_t eventID, Buffer eventData)
{
}

void Entity::Created()
{
}

void Entity::AddedToWorld(World *world)
{
}

void Entity::Tick(double delta)
{
}

void Entity::Render()
{
}

void Entity::Destroyed()
{
}

void Entity::Destroy()
{
	m_World->DestroyEntity(this);
}

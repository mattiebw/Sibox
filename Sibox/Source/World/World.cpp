﻿#include "siboxpch.h"
#include "World/World.h"

#include <ranges>

#include "Core/Random.h"
#include "Game/Player.h"
#include "World/TileMap.h"
#include "World/Entity.h"

World::World()
{
}

void World::UpdateEntityUUID(UUID oldID, UUID newID)
{
	SIBOX_ASSERT(m_Entities.contains(oldID) && "Entity does not exist");
	SIBOX_ASSERT(!m_Entities.contains(newID) && "Entity with new ID already exists");

	Ref<Entity> entity = m_Entities[oldID];
	m_Entities.erase(oldID);
	m_Entities[newID] = entity;
}

void World::DestroyEntity(UUID id)
{
	auto it = m_Entities.find(id);

	if (it == m_Entities.end())
	{
		SIBOX_ERROR("Entity with ID {} does not exist", id);
		return;
	}

	it->second->Destroyed();
	m_Entities.erase(it);
}

void World::DestroyEntity(Entity *entity)
{
	DestroyEntity(entity->GetUUID());
}

Ref<TileMap> World::CreateTileMap(const Ref<TileSet> &tileSet, int chunkWidth, int chunkHeight)
{
	auto tilemap = CreateRef<TileMap>(tileSet, chunkWidth, chunkHeight);
	m_TileMaps.emplace_back(tilemap);
	return tilemap;
}

void World::Tick(double delta)
{
	m_UnscaledDelta = delta;
	m_Delta         = delta * m_TimeScale;

	// MW @todo: This sucks; players should either update the tilemap themselves (or call a function on the world that does it),
	// or the World should keep a list of all the players. Regardless, we should not be interating over all entities to find
	// all the players every frame.
	std::vector<glm::ivec2> playerPositions;
	for (const auto &entity : m_Entities | std::views::values)
	{
		entity->Tick(m_Delta);

		if (entity->GetTypeID() == Player::s_EntityTypeID)
		{
			Player *player = static_cast<Player*>(entity.get());
			playerPositions.push_back(player->GetPosition());
		}
	}
	
	for (const auto& tilemap : m_TileMaps)
		tilemap->UpdateChunkLoading(playerPositions);
}

void World::Render()
{
	for (const auto& tilemap : m_TileMaps)
		tilemap->Render();

	for (const auto &entity : m_Entities | std::views::values)
		entity->Render();
}

bool World::RectOverlapsAnySolidTile(const FRect &rect) const
{
	for (const auto& tilemap : m_TileMaps)
	{
		if (tilemap->RectOverlapsSolidTile(rect))
			return true;
	}
	
	return false;
}

void World::Clean()
{
	m_TileMaps.clear();
	m_Entities.clear();
}

#pragma once

#include "Entity.h"
#include "Physics/PhysicsScene.h"

class PhysicsScene;
class TileMap;
class Entity;

class World
{
public:
	World();

	void AddEntity(const Ref<Entity> &entity)
	{
		m_Entities[entity->GetUUID()] = entity;
		// MW @todo: check if the entity was already in a world.
		entity->SetWorld(this);
		entity->AddedToWorld(this);
	}

	template <typename T, typename... Args>
	Ref<T> AddEntity(Args &&... args)
	{
		static_assert(std::is_base_of_v<Entity, T>, "T must inherit from Entity");
		UUID   uuid;
		Ref<T> entity = CreateRef<T>(std::forward<Args>(args)...);
		entity->SetUUID(uuid);
		entity->Created();
		AddEntity(entity);

		switch (m_NetworkType)
		{
		case NetworkType::Standalone:
		case NetworkType::Client:
			entity->m_EntityNetworkType = EntityNetworkType::LocalOnly;
			break;
		case NetworkType::DedicatedServer:
		case NetworkType::ListenServer:
			entity->m_EntityNetworkType = EntityNetworkType::LocalReplicated;
			break;
		}

		return entity;
	}

	template <typename T, typename... Args>
	Ref<T> AddEntityWithNetmode(EntityNetworkType netMode, Args &&... args)
	{
		Ref<T> entity               = AddEntity(std::forward<Args>(args)...);
		entity->m_EntityNetworkType = netMode;
	}

	void UpdateEntityUUID(UUID oldID, UUID newID);
	void DestroyEntity(UUID id);
	void DestroyEntity(const Entity *entity);

	Ref<TileMap> CreateTileMap(const Ref<class TileSet> &tileSet, int chunkWidth = 32, int chunkHeight = 32);

	void Tick(f64 delta);
	void Render();

	NODISCARD bool RectOverlapsAnySolidTile(const RectF &rect) const;

	void Clean();

	NODISCARD FORCEINLINE PhysicsScene& GetPhysicsScene() { return m_PhysicsScene; }
	NODISCARD FORCEINLINE NetworkType   GetNetworkType() const { return m_NetworkType; }
	NODISCARD FORCEINLINE f64           GetDelta() const { return m_Delta; }
	NODISCARD FORCEINLINE f64           GetUnscaledDelta() const { return m_UnscaledDelta; }
	NODISCARD FORCEINLINE f64           GetTimeScale() const { return m_TimeScale; }
	NODISCARD FORCEINLINE f64*          GetTimeScaleRef() { return &m_TimeScale; }
	FORCEINLINE void                    SetTimeScale(f64 timeScale)
	{
		m_TimeScale = timeScale;
		m_Delta     = m_UnscaledDelta * m_TimeScale;
	}

private:
	PhysicsScene                          m_PhysicsScene;
	std::unordered_map<UUID, Ref<Entity>> m_Entities;
	std::vector<Ref<TileMap>>             m_TileMaps;
	f64                                   m_TimeScale     = 1.0f;
	f64                                   m_Delta         = 0;
	f64                                   m_UnscaledDelta = 0;
	NetworkType                           m_NetworkType   = NetworkType::Standalone;

	friend class Application;
};

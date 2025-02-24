#pragma once

#include "Entity.h"

class TileMap;
class Entity;

class World
{
public:
	World();

	void AddEntity(const Ref<Entity>& entity)
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
		Ref<T> entity = AddEntity(std::forward<Args>(args)...);
		entity->m_EntityNetworkType = netMode;
	}

	void UpdateEntityUUID(UUID oldID, UUID newID);
	void DestroyEntity(UUID id);
	void DestroyEntity(Entity *entity);

	Ref<TileMap> CreateTileMap(const Ref<class TileSet> &tileSet, int chunkWidth = 32, int chunkHeight = 32);

	void Tick(double delta);
	void Render();

	NODISCARD bool RectOverlapsAnySolidTile(const FRect &rect) const;

	void Clean();

	NODISCARD FORCEINLINE NetworkType GetNetworkType() const { return m_NetworkType; }
	NODISCARD FORCEINLINE double  GetDelta() const { return m_Delta; }
	NODISCARD FORCEINLINE double  GetUnscaledDelta() const { return m_UnscaledDelta; }
	NODISCARD FORCEINLINE double  GetTimeScale() const { return m_TimeScale; }
	NODISCARD FORCEINLINE double* GetTimeScaleRef() { return &m_TimeScale; }
	FORCEINLINE void              SetTimeScale(double timeScale)
	{
		m_TimeScale = timeScale;
		m_Delta     = m_UnscaledDelta * m_TimeScale;
	}

private:
	std::unordered_map<UUID, Ref<Entity>> m_Entities;
	std::vector<Ref<TileMap>>             m_TileMaps;
	double                                m_TimeScale     = 1.0f;
	double                                m_Delta         = 0;
	double                                m_UnscaledDelta = 0;
	NetworkType                           m_NetworkType = NetworkType::Standalone;

	friend class Application;
};

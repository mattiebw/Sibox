#pragma once

#include "Render/Texture.h"
#include "World/Entity.h"

class Viewport;
class Camera;

class Player : public Entity
{
	ENTITY_CLASS_BODY(Player);

	void Created() override;
	void AddedToWorld(World *world) override;
	void Tick(f64 delta) override;
	void Render() override;

	NODISCARD FORCEINLINE Ref<Camera>& GetCamera() { return m_Camera; }

	RPC(Multicast, SendMovement, Vector2F position, Vector2F velocity);
	RPC(Multicast, OnDeath);

	std::string Name = "Qerma";
	
	STEAM_CALLBACK( Player, OnPersonaNameChange, PersonaStateChange_t );

private:
	f32         m_Time = 0;
	Ref<Texture>  m_Texture;
	Ref<Viewport> m_Viewport;
	Ref<Camera>   m_Camera;
};
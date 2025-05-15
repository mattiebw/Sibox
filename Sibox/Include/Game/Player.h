#pragma once

#include "World/Entity.h"

class Viewport;
class Camera;

class Player : public Entity
{
	ENTITY_CLASS_BODY(Player);

	void Created() override;
	void AddedToWorld(World *world) override;
	void Tick(f64 delta) override;

	NODISCARD FORCEINLINE Ref<Camera>& GetCamera() { return m_Camera; }

private:
	f32           m_Time = 0;
	Ref<Viewport> m_Viewport;
	Ref<Camera>   m_Camera;
};

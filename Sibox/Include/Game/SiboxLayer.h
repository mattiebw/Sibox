#pragma once

#include "Core/Layer.h"

class Entity;
class World;
class Player;
class MeshEntity;

class SiboxLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void Update(f64 delta) override;
	void Render(f64 delta) override;
	void RenderImGUI(f64 delta) override;

	void AddSpheres();

protected:
	std::vector<Entity*> m_DynamicBodies;
	Ref<World> m_World;
	Ref<MeshEntity> m_Mesh;
	Ref<Player> m_Player;
};

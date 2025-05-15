#pragma once
#include "Core/Layer.h"

class MeshEntity;
class Mesh;
class OldPlayer;
class World;
class Camera;

class SiboxLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void Update(f64 delta) override;
	void Render(f64 delta) override;
	void RenderImGUI(f64 delta) override;

protected:
	Ref<MeshEntity> m_Mesh;
	Ref<OldPlayer> m_Player;
};

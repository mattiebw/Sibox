#pragma once

#include "Core/Layer.h"

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

protected:
	Ref<MeshEntity> m_Mesh;
	Ref<Player> m_Player;
};

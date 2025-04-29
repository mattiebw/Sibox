#pragma once
#include "Core/Layer.h"

class Player;
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
	Ref<Player> m_Player;
};

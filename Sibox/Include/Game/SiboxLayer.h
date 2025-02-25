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
	void Update(double delta) override;
	void Render(double delta) override;
	void RenderImGUI(double delta) override;

protected:
	Ref<Player> m_Player;
	Transform   m_TextTransform;
};

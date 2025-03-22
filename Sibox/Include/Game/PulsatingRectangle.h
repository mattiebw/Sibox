#pragma once
#include "World/Entity.h"

class PulsatingRectangle : public Entity
{
public:
	ENTITY_CLASS_BODY(PulsatingRectangle)

	void Created() override;
	void Tick(double delta) override;
	void Render() override;

private:
	Vector3F m_Color;
	float    m_Time = 0;
};

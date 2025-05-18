#pragma once

#include "Render/Material.h"
#include "World/Entity.h"

struct Body;

class SphereEntity : public Entity
{
	ENTITY_CLASS_BODY(SphereEntity);

	void Setup(f32 radius = 1.0f, f32 invMass = 1.0f);
	
	virtual void Tick(f64 delta) override;
	virtual void Render() override;
	virtual void Destroyed() override;

	void SetPosition(Vector3F pos);

protected:
	Material m_Material;
	Body* m_Body;
};

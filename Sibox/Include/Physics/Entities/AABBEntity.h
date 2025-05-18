#pragma once

#include "Render/Material.h"
#include "World/Entity.h"

struct Body;

class AABBEntity : public Entity
{
	ENTITY_CLASS_BODY(AABBEntity);
	
	void Setup(Vector3F size = {1.0f, 1.0f, 1.0f}, f32 invMass = 1.0f);

	virtual void Tick(f64 delta) override;
	virtual void Render() override;
	virtual void Destroyed() override;

	void SetBodyPosition(Vector3F pos);

protected:
	Material m_Material;
	Body *   m_Body;
};

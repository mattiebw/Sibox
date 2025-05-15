#pragma once

#include "World/Entity.h"

class Mesh;

class MeshEntity : public Entity
{
	ENTITY_CLASS_BODY(MeshEntity);

	void Render() override;

	FORCEINLINE void SetMesh(const Ref<Mesh> &mesh)
	{
		m_Mesh = mesh;
	}

	NODISCARD FORCEINLINE const Ref<Mesh>& GetMesh() const
	{
		return m_Mesh;
	}

private:
	Ref<Mesh> m_Mesh;
};

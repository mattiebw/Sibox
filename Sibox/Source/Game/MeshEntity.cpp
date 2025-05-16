#include "siboxpch.h"
#include "Game/MeshEntity.h"

#include "Core/Application.h"

void MeshEntity::Render()
{
	Application::GetRenderer()->DrawMeshWireframe(m_Mesh.get(), EntityTransform.GetTransformationMatrix());
}

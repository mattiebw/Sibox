#include "siboxpch.h"
#include "Game/Assets.h"

#include "Render/Texture.h"
#include "Render/Mesh.h"

Ref<Texture> Assets::GreenGrid;
Ref<Mesh>    Assets::Sphere;

void Assets::Initialise()
{
	GreenGrid = CreateRef<Texture>("Content/Textures/Dev/Green/texture_09.png");

	Sphere = CreateRef<Mesh>(MeshSpecification());
	Sphere->LoadFromFile("Content/Meshes/Sphere.mesh");
}

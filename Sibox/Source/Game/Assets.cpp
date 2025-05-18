#include "siboxpch.h"
#include "Game/Assets.h"

#include "Render/Texture.h"
#include "Render/Mesh.h"

Ref<Texture> Assets::GreenGrid;
Ref<Texture> Assets::GreyGrid;
Ref<Mesh>    Assets::Cube;
Ref<Mesh>    Assets::Sphere;
Ref<Mesh>    Assets::VeryFuckingDetailedSphere;

void Assets::Initialise()
{
	TextureSpecification grid = {};
	grid.Wrap = WrapMode::Repeat;
	
	GreenGrid = CreateRef<Texture>("Content/Textures/Dev/Green/texture_09.png", grid);
	GreyGrid = CreateRef<Texture>("Content/Textures/Dev/Dark/texture_09.png", grid);

	Cube = CreateRef<Mesh>(MeshSpecification());
	Cube->LoadFromFile("Content/Meshes/Cube.mesh");
	Sphere = CreateRef<Mesh>(MeshSpecification());
	Sphere->LoadFromFile("Content/Meshes/Sphere.mesh");
	VeryFuckingDetailedSphere = CreateRef<Mesh>(MeshSpecification());
	VeryFuckingDetailedSphere->LoadFromFile("Content/Meshes/VeryFuckingDetailedSphere.mesh");
}

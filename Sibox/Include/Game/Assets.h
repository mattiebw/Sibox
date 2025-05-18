#pragma once

class Texture;
class Mesh;

class Assets
{
public:
	static void Initialise();

	static Ref<Texture> GreenGrid;
	static Ref<Texture> GreyGrid;
	
	static Ref<Mesh> Cube;
	static Ref<Mesh> Sphere;
	static Ref<Mesh> VeryFuckingDetailedSphere;
};

#pragma once

class Texture;
class Mesh;

class Assets
{
public:
	static void Initialise();

	static Ref<Texture> GreenGrid;
	
	static Ref<Mesh> Sphere;
};

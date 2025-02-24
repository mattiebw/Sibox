#pragma once

class TileSet;

class TileSets
{
public:
	static void Init();
	static void Shutdown();

	static Ref<TileSet> MainTileSet;
	static uint16_t     Empty, Grass, Flowers, Sunflowers, Lilies, Roses, 
						StoneWall, MoutainStone, Snow, Sand, Mud1, Mud2, Mud3, Mud4,
						DWater1, DWater2, DWater3, DWater4, DWater5, DWater6, LWater1, LWater2, LWater3, LWater4, LWater5, LWater6;
};

#include "siboxpch.h"
#include "World/TileSets.h"

#include "Core/Application.h"
#include "World/TileSet.h"
#include "Render/Texture.h"
#include "Render/SpriteSheet.h"

Ref<TileSet> TileSets::MainTileSet = nullptr;
uint16_t     TileSets::Empty       = 0;
uint16_t     TileSets::Grass       = 0;
uint16_t     TileSets::Flowers     = 0;
uint16_t     TileSets::Sunflowers  = 0;
uint16_t     TileSets::Lilies      = 0;
uint16_t     TileSets::Roses       = 0;
uint16_t     TileSets::StoneWall   = 0;
uint16_t     TileSets::MoutainStone= 0;
uint16_t     TileSets::Snow        = 0;
uint16_t     TileSets::Sand        = 0;
uint16_t     TileSets::Mud1        = 0;
uint16_t     TileSets::Mud2        = 0;
uint16_t     TileSets::Mud3        = 0;
uint16_t     TileSets::Mud4        = 0;
uint16_t     TileSets::DWater1     = 0;
uint16_t     TileSets::DWater2     = 0;
uint16_t     TileSets::DWater3     = 0;
uint16_t     TileSets::DWater4     = 0;
uint16_t     TileSets::DWater5     = 0;
uint16_t     TileSets::DWater6     = 0;
uint16_t     TileSets::LWater1     = 0;
uint16_t     TileSets::LWater2	   = 0;
uint16_t     TileSets::LWater3     = 0;
uint16_t     TileSets::LWater4     = 0;
uint16_t     TileSets::LWater5     = 0;
uint16_t     TileSets::LWater6     = 0;


void TileSets::Init()
{
	if (Application::Get()->HasFrontend())
	{
		TextureSpecification spec;
		spec.MagFilter = FilterMode::Nearest;
		spec.MinFilter = FilterMode::Nearest;
		spec.Wrap = WrapMode::ClampToEdge;
		spec.FlipVertically = false;
		Ref<Texture>     texture     = CreateRef<Texture>("Content/Textures/TerrainSpritesheet.png", spec);
		
		Ref<SpriteSheet> spriteSheet = CreateRef<SpriteSheet>(texture);
		spriteSheet->CreateTilesFromTileSize(16, 16);
		MainTileSet = CreateRef<TileSet>(spriteSheet);
	} else
	{
		MainTileSet = CreateRef<TileSet>(nullptr);
	}

	TileData grassData;
	grassData.SpriteIndex = 0;
	grassData.IsSolid     = false;
	Grass                 = MainTileSet->AddTile(grassData);

	TileData FlowersData;
	FlowersData.SpriteIndex = 10;
	FlowersData.IsSolid = false;
	Flowers = MainTileSet->AddTile(FlowersData);

	TileData SunflowersData;
	SunflowersData.SpriteIndex = 29;
	SunflowersData.IsSolid = false;
	Sunflowers = MainTileSet->AddTile(SunflowersData);

	TileData LiliesData;
	LiliesData.SpriteIndex = 30;
	LiliesData.IsSolid = false;
	Lilies = MainTileSet->AddTile(LiliesData);

	TileData RosesData;
	RosesData.SpriteIndex = 31;
	RosesData.IsSolid = false;
	Roses = MainTileSet->AddTile(RosesData);

	TileData StoneWallData;
	StoneWallData.SpriteIndex = 8;
	StoneWallData.IsSolid     = true;
	StoneWall                 = MainTileSet->AddTile(StoneWallData);

	TileData MoutainStoneData;
	MoutainStoneData.SpriteIndex = 9;
	MoutainStoneData.IsSolid     = true;
	MoutainStone                 = MainTileSet->AddTile(MoutainStoneData);

	TileData SnowData;
	SnowData.SpriteIndex = 37;
	SnowData.IsSolid = true;
	Snow = MainTileSet->AddTile(SnowData);

	TileData SandData;
	SandData.SpriteIndex = 1;
	SandData.IsSolid = false;
	Sand = MainTileSet->AddTile(SandData);

	TileData Mud1Data;
	Mud1Data.SpriteIndex = 32;
	Mud1Data.IsSolid = false;
	Mud1 = MainTileSet->AddTile(Mud1Data);

	TileData Mud2Data;
	Mud2Data.SpriteIndex = 33;
	Mud2Data.IsSolid = false;
	Mud2 = MainTileSet->AddTile(Mud2Data);

	TileData Mud3Data;
	Mud3Data.SpriteIndex = 34;
	Mud3Data.IsSolid = false;
	Mud3 = MainTileSet->AddTile(Mud3Data);

	TileData Mud4Data;
	Mud4Data.SpriteIndex = 35;
	Mud4Data.IsSolid = false;
	Mud4 = MainTileSet->AddTile(Mud4Data);

	TileData DWater1Data;
	DWater1Data.SpriteIndex = 18;
	DWater1Data.IsSolid = true;
	DWater1 = MainTileSet->AddTile(DWater1Data);

	TileData DWater2Data;
	DWater2Data.SpriteIndex = 19;
	DWater2Data.IsSolid = true;
	DWater2 = MainTileSet->AddTile(DWater2Data);

	TileData DWater3Data;
	DWater3Data.SpriteIndex = 20;
	DWater3Data.IsSolid = true;
	DWater3 = MainTileSet->AddTile(DWater3Data);

	TileData DWater4Data;
	DWater4Data.SpriteIndex = 21;
	DWater4Data.IsSolid = true;
	DWater4 = MainTileSet->AddTile(DWater4Data);

	TileData DWater5Data;
	DWater5Data.SpriteIndex = 22;
	DWater5Data.IsSolid = true;
	DWater5 = MainTileSet->AddTile(DWater5Data);

	TileData DWater6Data;
	DWater6Data.SpriteIndex = 23;
	DWater6Data.IsSolid = true;
	DWater6 = MainTileSet->AddTile(DWater6Data);

	TileData LWater1Data;
	LWater1Data.SpriteIndex = 2;
	LWater1Data.IsSolid = false;
	LWater1 = MainTileSet->AddTile(LWater1Data);

	TileData LWater2Data;
	LWater2Data.SpriteIndex = 3;
	LWater2Data.IsSolid = false;
	LWater2 = MainTileSet->AddTile(LWater2Data);

	TileData LWater3Data;
	LWater3Data.SpriteIndex = 4;
	LWater3Data.IsSolid = false;
	LWater3 = MainTileSet->AddTile(LWater3Data);

	TileData LWater4Data;
	LWater4Data.SpriteIndex = 5;
	LWater4Data.IsSolid = false;
	LWater4 = MainTileSet->AddTile(LWater4Data);

	TileData LWater5Data;
	LWater5Data.SpriteIndex = 6;
	LWater5Data.IsSolid = false;
	LWater5 = MainTileSet->AddTile(LWater5Data);

	TileData LWater6Data;
	LWater6Data.SpriteIndex = 7;
	LWater6Data.IsSolid = false;
	LWater6 = MainTileSet->AddTile(LWater6Data);
}

void TileSets::Shutdown()
{
	MainTileSet = nullptr;
}

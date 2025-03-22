#include "siboxpch.h"
#include "World/TileSet.h"

#include "Render/SpriteSheet.h"

TileSet::TileSet(const Ref<SpriteSheet> &spritesheet)
	: m_Spritesheet(spritesheet)
{
}

TileSet::~TileSet()
{
}

u16 TileSet::AddTile(const TileData &data)
{
	m_Tiles.push_back(data);
	return static_cast<u16>(m_Tiles.size());
}

SpriteSheetSprite TileSet::GetSpriteForTile(u16 tile) const
{
	SIBOX_ASSERT(tile > 0 && tile <= m_Tiles.size() && "Invalid tile index");
	return m_Spritesheet->GetSprite(m_Tiles[tile - 1].SpriteIndex);
}

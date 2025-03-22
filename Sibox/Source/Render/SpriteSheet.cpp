#include "siboxpch.h"
#include "Render/SpriteSheet.h"

#include "Render/Texture.h"

SpriteSheet::SpriteSheet(const Ref<Texture> &texture)
{
	m_Texture = texture;
}

u32 SpriteSheet::CreateSprite(u32 x, u32 y, u32 width, u32 height)
{
	SIBOX_ASSERT(m_Texture != nullptr && "SpriteSheet has no texture!");

	u32  index = static_cast<u32>(m_Sprites.size());
	Vector2F coordsMin, coordsMax;
	coordsMin.X = static_cast<f32>(x) / static_cast<f32>(m_Texture->GetWidth());
	coordsMin.Y = static_cast<f32>(y) / static_cast<f32>(m_Texture->GetHeight());
	coordsMax.X = static_cast<f32>(x + width) / static_cast<f32>(m_Texture->GetWidth());
	coordsMax.Y = static_cast<f32>(y + height) / static_cast<f32>(m_Texture->GetHeight());
	m_Sprites.push_back({coordsMin, coordsMax});

	return index;
}

u32 SpriteSheet::CreateTilesFromTileSize(u32 tileWidth, u32 tileHeight)
{
	SIBOX_ASSERT(m_Texture != nullptr && "TileSet has no texture!");
	SIBOX_ASSERT(
		m_Texture->GetWidth() % tileWidth == 0 && m_Texture->GetHeight() % tileHeight == 0 &&
		"Tile width and height of the texture must be a multiple of the tile width and height!");

	s32 tilesX = m_Texture->GetWidth() / tileWidth;
	s32 tilesY = m_Texture->GetHeight() / tileHeight;
	for (s32 y = 0; y < tilesY; y++)
		for (s32 x = 0; x < tilesX; x++)
			CreateSprite(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

	return static_cast<u32>(m_Sprites.size());
}

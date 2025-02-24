#include "papipch.h"
#include "Render/SpriteSheet.h"

#include "Render/Texture.h"

SpriteSheet::SpriteSheet(const Ref<Texture> &texture)
{
	m_Texture = texture;
}

uint32_t SpriteSheet::CreateSprite(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	PAPI_ASSERT(m_Texture != nullptr && "SpriteSheet has no texture!");

	uint32_t  index = static_cast<uint32_t>(m_Sprites.size());
	glm::vec2 coordsMin, coordsMax;
	coordsMin.x = static_cast<float>(x) / static_cast<float>(m_Texture->GetWidth());
	coordsMin.y = static_cast<float>(y) / static_cast<float>(m_Texture->GetHeight());
	coordsMax.x = static_cast<float>(x + width) / static_cast<float>(m_Texture->GetWidth());
	coordsMax.y = static_cast<float>(y + height) / static_cast<float>(m_Texture->GetHeight());
	m_Sprites.push_back({coordsMin, coordsMax});

	return index;
}

uint32_t SpriteSheet::CreateTilesFromTileSize(uint32_t tileWidth, uint32_t tileHeight)
{
	PAPI_ASSERT(m_Texture != nullptr && "TileSet has no texture!");
	PAPI_ASSERT(
		m_Texture->GetWidth() % tileWidth == 0 && m_Texture->GetHeight() % tileHeight == 0 &&
		"Tile width and height of the texture must be a multiple of the tile width and height!");

	int32_t tilesX = m_Texture->GetWidth() / tileWidth;
	int32_t tilesY = m_Texture->GetHeight() / tileHeight;
	for (int32_t y = 0; y < tilesY; y++)
		for (int32_t x = 0; x < tilesX; x++)
			CreateSprite(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

	return static_cast<uint32_t>(m_Sprites.size());
}

#pragma once

class Texture;

struct SpriteSheetSprite
{
	glm::vec2 TexCoordsMin;
	glm::vec2 TexCoordsMax;
};

// MW @todo: redesign for animation?
class SpriteSheet
{
public:
	SpriteSheet(const Ref<Texture> &texture);

	uint32_t CreateSprite(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	uint32_t CreateTilesFromTileSize(uint32_t tileWidth, uint32_t tileHeight);

	NODISCARD FORCEINLINE const Ref<Texture>&      GetTexture() const { return m_Texture; }
	NODISCARD FORCEINLINE const SpriteSheetSprite& GetSprite(uint32_t id) const { return m_Sprites[id]; }

private:
	std::vector<SpriteSheetSprite> m_Sprites;
	Ref<Texture>                   m_Texture;
};

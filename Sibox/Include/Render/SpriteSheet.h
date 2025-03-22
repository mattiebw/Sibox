#pragma once

class Texture;

struct SpriteSheetSprite
{
	Vector2F TexCoordsMin;
	Vector2F TexCoordsMax;
};

// MW @todo: redesign for animation?
class SpriteSheet
{
public:
	SpriteSheet(const Ref<Texture> &texture);

	u32 CreateSprite(u32 x, u32 y, u32 width, u32 height);
	u32 CreateTilesFromTileSize(u32 tileWidth, u32 tileHeight);

	NODISCARD FORCEINLINE const Ref<Texture>&      GetTexture() const { return m_Texture; }
	NODISCARD FORCEINLINE const SpriteSheetSprite& GetSprite(u32 id) const { return m_Sprites[id]; }

private:
	std::vector<SpriteSheetSprite> m_Sprites;
	Ref<Texture>                   m_Texture;
};

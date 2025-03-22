#pragma once

struct SpriteSheetSprite;
class SpriteSheet;

struct TileData
{
	u16 SpriteIndex;
	bool     IsSolid = false;
};

class TileSet
{
public:
	TileSet(const Ref<SpriteSheet> &spritesheet);
	~TileSet();
	TileSet(const TileSet &other)                = delete;
	TileSet(TileSet &&other) noexcept            = delete;
	TileSet& operator=(const TileSet &other)     = delete;
	TileSet& operator=(TileSet &&other) noexcept = delete;

	u16                        AddTile(const TileData &data);
	NODISCARD FORCEINLINE TileData& GetTile(u16 tileIndex) { return tileIndex == 0 ? m_Empty : m_Tiles[tileIndex - 1]; }

	NODISCARD FORCEINLINE const Ref<SpriteSheet>& GetSpritesheet() const { return m_Spritesheet; }

	NODISCARD SpriteSheetSprite GetSpriteForTile(u16 tile) const;

protected:
	TileData m_Empty;
	std::vector<TileData> m_Tiles;

	Ref<SpriteSheet> m_Spritesheet;
};

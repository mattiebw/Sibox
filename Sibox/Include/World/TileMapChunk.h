#pragma once

struct TileData;
class VertexBuffer;
class VertexArray;
class TileMap;

struct TileShaderData
{
	Vector2F TopLeftTexCoord;
	f32      Rot = 0;
};

class TileMapChunk
{
public:
	TileMapChunk();
	TileMapChunk(TileMap *tileMap, Vector2I position, Vector2I size = {32, 32});
	~TileMapChunk();
	TileMapChunk(const TileMapChunk &other)                = delete;
	TileMapChunk(TileMapChunk &&other) noexcept            = delete;
	TileMapChunk& operator=(const TileMapChunk &other)     = delete;
	TileMapChunk& operator=(TileMapChunk &&other) noexcept = delete;

	NODISCARD FORCEINLINE Vector2I                GetSize() const { return m_Size; }
	NODISCARD FORCEINLINE Vector2I                GetPosition() const { return m_Position; }
	NODISCARD FORCEINLINE TileMap*                GetTileMap() const { return m_TileMap; }
	NODISCARD FORCEINLINE const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
	NODISCARD FORCEINLINE RectF                   GetBounds() const
	{
		return RectF(static_cast<f32>(m_Position.X), static_cast<f32>(m_Position.Y), static_cast<f32>(m_Size.X),
		             static_cast<f32>(m_Size.Y));
	}

	NODISCARD FORCEINLINE u32 GetTile(s32 x, s32 y) const { return m_Tiles[y * m_Size.X + x]; }
	NODISCARD TileData&       GetTileDataForTile(s32 x, s32 y) const;
	void                      SetTile(s32 x, s32 y, u32 tile);

	void UpdateTileData();

protected:
	// Position is the top-left corner of the chunk in world space. Size is the size of the chunk in tiles.
	Vector2I m_Position, m_Size;
	u32 *    m_Tiles;
	bool     m_DataDirty = true;

	Ref<VertexArray>  m_VertexArray;
	Ref<VertexBuffer> m_TileDataBuffer;
	TileShaderData *  m_TileShaderData = nullptr;

	TileMap *m_TileMap;
};

#pragma once
#include "ChunkProvider.h"
#include "TileMapChunk.h"

#include "TileSet.h"

class TileMap
{
public:
	TileMap(const Ref<TileSet> &tileSet, int chunkWidth = 32, int chunkHeight = 32);

	void               SetTile(int x, int y, uint32_t tile, bool canCreateChunk = false);
	NODISCARD uint32_t GetTile(int x, int y, bool canCreateChunk = true);
	NODISCARD uint32_t GetTile(int x, int y) const;

	NODISCARD FORCEINLINE uint32_t            GetChunkWidth() const { return m_ChunkSize.X; }
	NODISCARD FORCEINLINE uint32_t            GetChunkHeight() const { return m_ChunkSize.Y; }
	NODISCARD FORCEINLINE const Ref<TileSet>& GetTileSet() const { return m_TileSet; }

	NODISCARD FORCEINLINE size_t GetLoadedChunkCount() const { return m_Chunks.size(); }

	NODISCARD TileMapChunk*       GetChunk(Vector2I index) { return m_Chunks.at(index).get(); }
	NODISCARD const TileMapChunk* GetChunk(Vector2I index) const { return m_Chunks.at(index).get(); }
	NODISCARD TileMapChunk*       GetChunkFromTileCoordinate(int x, int y, bool canCreateChunk = true);
	NODISCARD TileMapChunk*       GetChunkFromTileCoordinate(int x, int y) const;
	NODISCARD TileMapChunk*       LoadChunk(Vector2I index); // Either load the chunk from disk, or generate it.
	NODISCARD bool                IsChunkLoaded(Vector2I index) const { return m_Chunks.contains(index); }

	void SetChunkProvider(const Ref<ChunkProvider> &chunkProvider);

	NODISCARD bool RectOverlapsSolidTile(const RectF &rect);

	void UpdateChunkLoading(const std::vector<Vector2I> &playerPositions);
	void Render();

	float Z = 0;

protected:
	// The key in our chunk map is the chunk coordinate.
	// For example, if the chunk size is 32x32 and the chunk coordinate is (1, 2),
	// then the top left tile in that chunk is tile (32, 64).
	Ref<ChunkProvider>                              m_ChunkProvider;
	std::unordered_map<Vector2I, Ref<TileMapChunk>> m_Chunks;
	Vector2I                                        m_ChunkSize;

	Ref<TileSet> m_TileSet;
};

#include "siboxpch.h"
#include "World/TileMap.h"

#include "Core/Application.h"
#include "Render/Camera.h"
#include "Render/Viewport.h"

TileMap::TileMap(const Ref<TileSet> &tileSet, int chunkWidth, int chunkHeight)
	: m_ChunkSize(chunkWidth, chunkHeight), m_TileSet(tileSet)
{
	m_ChunkProvider = CreateRef<DefaultChunkProvider>();
}

void TileMap::SetTile(int x, int y, u32 tile, bool canCreateChunk)
{
	Vector2I tileCoords = Vector2I(std::floor(x / m_ChunkSize.X), std::floor(y / m_ChunkSize.Y));
	auto     chunk      = m_Chunks.find(tileCoords);
	if (chunk == m_Chunks.end())
	{
		if (canCreateChunk)
		{
			auto generatedChunk = LoadChunk(tileCoords);
			generatedChunk->SetTile(x % m_ChunkSize.X, y % m_ChunkSize.Y, tile);
		}
	}
	else
		chunk->second->SetTile(x % m_ChunkSize.X, y % m_ChunkSize.Y, tile);
}

u32 TileMap::GetTile(int x, int y, bool canCreateChunk)
{
	Vector2I tileCoords = Vector2I(std::floor(x / m_ChunkSize.X), std::floor(y / m_ChunkSize.Y));
	auto     chunk      = m_Chunks.find(tileCoords);
	if (chunk == m_Chunks.end())
	{
		if (canCreateChunk)
		{
			auto generatedChunk = LoadChunk(tileCoords);
			return generatedChunk->GetTile(x % m_ChunkSize.X, y % m_ChunkSize.Y);
		}

		return static_cast<u32>(-1);
	}
	return chunk->second->GetTile(x % m_ChunkSize.X, y % m_ChunkSize.Y);
}

u32 TileMap::GetTile(int x, int y) const
{
	Vector2I tileCoords = Vector2I(std::floor(x / m_ChunkSize.X), std::floor(y / m_ChunkSize.Y));
	auto     chunk      = m_Chunks.find(tileCoords);
	return chunk == m_Chunks.end()
		       ? static_cast<u32>(-1)
		       : chunk->second->GetTile(x % m_ChunkSize.X, y % m_ChunkSize.Y);
}

void TileMap::UpdateChunkLoading(const std::vector<Vector2I> &playerPositions)
{
	for (Vector2I pos : playerPositions)
	{
		// This is where we load chunks around the player.
		// We should probably have a way to unload chunks too.

		pos /= m_ChunkSize;
		for (int y = -2; y <= 1; y++)
		{
			for (int x = -2; x <= 1; x++)
			{
				LoadChunk(pos + Vector2I(x, y));
			}
		}
	}
}

void TileMap::Render()
{
	for (auto chunk : m_Chunks)
	{
		if (!Application::GetRenderer()->GetCurrentViewport()->GetCamera()->RectOverlapsCamera(
			chunk.second->GetBounds()))
		{
			continue;
		}

		Application::GetRenderer()->GetTilemapRenderer().DrawTileMapChunk(
			Vector3F(chunk.second->GetPosition(), Z), chunk.second.get());

		Application::GetQuadRenderer()->DrawRectangleLines(chunk.second->GetBounds(), {1.0f, 0.0f, 0.0f, 1.0f}, 0.1f);
	}
}

TileMapChunk* TileMap::GetChunkFromTileCoordinate(int x, int y, bool canCreateChunk)
{
	Vector2I tileCoords = Vector2I(std::floor(static_cast<f32>(x) / m_ChunkSize.X),
	                               std::floor(static_cast<f32>(y) / m_ChunkSize.Y));
	auto chunk = m_Chunks.find(tileCoords);
	if (chunk == m_Chunks.end())
	{
		if (canCreateChunk)
		{
			auto generatedChunk = LoadChunk(tileCoords);
			return generatedChunk;
		}
		return nullptr;
	}
	return chunk->second.get();
}

TileMapChunk* TileMap::GetChunkFromTileCoordinate(int x, int y) const
{
	Vector2I tileCoords = Vector2I(std::floor(static_cast<f32>(x) / m_ChunkSize.X),
	                               std::floor(static_cast<f32>(y) / m_ChunkSize.Y));
	auto chunk = m_Chunks.find(tileCoords);
	return chunk == m_Chunks.end() ? nullptr : chunk->second.get();
}

TileMapChunk* TileMap::LoadChunk(Vector2I index)
{
	if (IsChunkLoaded(index))
		return GetChunk(index);

	// MW @todo: Check to see if the chunk is on disk, if it is load and return it.

	// It's not on disk, lets generate one.

	Ref<TileMapChunk> chunk = CreateRef<TileMapChunk>(this, index * m_ChunkSize, m_ChunkSize);
	for (int y = 0; y < m_ChunkSize.Y; y++)
	{
		for (int x = 0; x < m_ChunkSize.X; x++)
		{
			chunk->SetTile(x, y, m_ChunkProvider->GetTileAt(x + index.X * m_ChunkSize.X,
			                                                y + index.Y * m_ChunkSize.Y));
		}
	}
	m_Chunks[index] = chunk;
	return chunk.get();
}

void TileMap::SetChunkProvider(const Ref<ChunkProvider> &chunkProvider)
{
	m_ChunkProvider = chunkProvider;
}

bool TileMap::RectOverlapsSolidTile(const RectF &rect)
{
	for (int y = static_cast<int>(std::floor(rect.Position.Y)); y < static_cast<int>(std::ceil(
		     rect.Position.Y + rect.Size.Y)); y++)
	{
		for (int x = static_cast<int>(std::floor(rect.Position.X)); x < static_cast<int>(std::ceil(
			     rect.Position.X + rect.Size.X)); x++)
		{
			// MW @todo @perf: Optimise! We shouldn't be looking up the chunk for every single tile.
			// We should try and check each chunk we overlap one by one first.
			// std::vector<Vector2I> GetOverlappedChunks(Rect)?

			TileMapChunk *chunk = GetChunkFromTileCoordinate(x, y, false);
			if (chunk != nullptr)
			{
				int cx = x % chunk->GetSize().X;
				if (cx < 0) cx = chunk->GetSize().X + cx;
				int cy = y % chunk->GetSize().Y;
				if (cy < 0) cy = chunk->GetSize().Y + cy;
				if (chunk->GetTileDataForTile(cx, cy).IsSolid)
					return true;
			}
		}
	}

	return false;
}

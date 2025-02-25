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

void TileMap::SetTile(int x, int y, uint32_t tile, bool canCreateChunk)
{
	glm::ivec2 tileCoords = glm::ivec2(std::floor(x / m_ChunkSize.x), std::floor(y / m_ChunkSize.y));
	auto       chunk      = m_Chunks.find(tileCoords);
	if (chunk == m_Chunks.end())
	{
		if (canCreateChunk)
		{
			auto generatedChunk = LoadChunk(tileCoords);
			generatedChunk->SetTile(x % m_ChunkSize.x, y % m_ChunkSize.y, tile);
		}
	}
	else
		chunk->second->SetTile(x % m_ChunkSize.x, y % m_ChunkSize.y, tile);
}

uint32_t TileMap::GetTile(int x, int y, bool canCreateChunk)
{
	glm::ivec2 tileCoords = glm::ivec2(std::floor(x / m_ChunkSize.x), std::floor(y / m_ChunkSize.y));
	auto       chunk      = m_Chunks.find(tileCoords);
	if (chunk == m_Chunks.end())
	{
		if (canCreateChunk)
		{
			auto generatedChunk = LoadChunk(tileCoords);
			return generatedChunk->GetTile(x % m_ChunkSize.x, y % m_ChunkSize.y);
		}

		return static_cast<uint32_t>(-1);
	}
	return chunk->second->GetTile(x % m_ChunkSize.x, y % m_ChunkSize.y);
}

uint32_t TileMap::GetTile(int x, int y) const
{
	glm::ivec2 tileCoords = glm::ivec2(std::floor(x / m_ChunkSize.x), std::floor(y / m_ChunkSize.y));
	auto       chunk      = m_Chunks.find(tileCoords);
	return chunk == m_Chunks.end()
		       ? static_cast<uint32_t>(-1)
		       : chunk->second->GetTile(x % m_ChunkSize.x, y % m_ChunkSize.y);
}

void TileMap::UpdateChunkLoading(const std::vector<glm::ivec2> &playerPositions)
{
	for (glm::ivec2 pos : playerPositions)
	{
		// This is where we load chunks around the player.
		// We should probably have a way to unload chunks too.

		pos /= m_ChunkSize;
		for (int y = -2; y <= 1; y++)
		{
			for (int x = -2; x <= 1; x++)
			{
				LoadChunk(pos + glm::ivec2(x, y));
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
			glm::vec3(chunk.second->GetPosition(), Z), chunk.second.get());

		Application::GetQuadRenderer()->DrawRectangleLines(chunk.second->GetBounds(), {1.0f, 0.0f, 0.0f, 1.0f}, 0.1f);
	}
}

TileMapChunk* TileMap::GetChunkFromTileCoordinate(int x, int y, bool canCreateChunk)
{
	glm::ivec2 tileCoords = glm::ivec2(std::floor(static_cast<float>(x) / m_ChunkSize.x),
	                                   std::floor(static_cast<float>(y) / m_ChunkSize.y));
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
	glm::ivec2 tileCoords = glm::ivec2(std::floor(static_cast<float>(x) / m_ChunkSize.x),
	                                   std::floor(static_cast<float>(y) / m_ChunkSize.y));
	auto chunk = m_Chunks.find(tileCoords);
	return chunk == m_Chunks.end() ? nullptr : chunk->second.get();
}

TileMapChunk* TileMap::LoadChunk(glm::ivec2 index)
{
	if (IsChunkLoaded(index))
		return GetChunk(index);

	// MW @todo: Check to see if the chunk is on disk, if it is load and return it.

	// It's not on disk, lets generate one.

	Ref<TileMapChunk> chunk = CreateRef<TileMapChunk>(this, index * m_ChunkSize, m_ChunkSize);
	for (int y = 0; y < m_ChunkSize.y; y++)
	{
		for (int x = 0; x < m_ChunkSize.x; x++)
		{
			chunk->SetTile(x, y, m_ChunkProvider->GetTileAt(x + index.x * m_ChunkSize.x,
			                                                y + index.y * m_ChunkSize.y));
		}
	}
	m_Chunks[index] = chunk;
	return chunk.get();
}

void TileMap::SetChunkProvider(const Ref<ChunkProvider> &chunkProvider)
{
	m_ChunkProvider = chunkProvider;
}

bool TileMap::RectOverlapsSolidTile(const FRect &rect)
{
	for (int y = static_cast<int>(std::floor(rect.Position.y)); y < static_cast<int>(std::ceil(
		     rect.Position.y + rect.Size.y)); y++)
	{
		for (int x = static_cast<int>(std::floor(rect.Position.x)); x < static_cast<int>(std::ceil(
			     rect.Position.x + rect.Size.x)); x++)
		{
			// MW @todo @perf: Optimise! We shouldn't be looking up the chunk for every single tile.
			// We should try and check each chunk we overlap one by one first.
			// std::vector<glm::ivec2> GetOverlappedChunks(Rect)?

			TileMapChunk *chunk = GetChunkFromTileCoordinate(x, y, false);
			if (chunk != nullptr)
			{
				int cx = x % chunk->GetSize().x;
				if (cx < 0) cx = chunk->GetSize().x + cx;
				int cy = y % chunk->GetSize().y;
				if (cy < 0) cy = chunk->GetSize().y + cy;
				if (chunk->GetTileDataForTile(cx, cy).IsSolid)
					return true;
			}
		}
	}

	return false;
}

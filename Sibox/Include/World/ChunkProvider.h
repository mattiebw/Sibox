#pragma once

#include "Core/Random.h"
#include <Vendor/FastNoiseLite.h>

class ChunkProvider
{
public:
	ChunkProvider()                                          = default;
	virtual ~ChunkProvider()                                 = default;
	ChunkProvider(const ChunkProvider &other)                = delete;
	ChunkProvider(ChunkProvider &&other) noexcept            = delete;
	ChunkProvider& operator=(const ChunkProvider &other)     = delete;
	ChunkProvider& operator=(ChunkProvider &&other) noexcept = delete;

	virtual uint16_t GetTileAt(int x, int y) const = 0;
};

class NullChunkProvider : public ChunkProvider
{
	uint16_t GetTileAt(int x, int y) const override
	{
		return 0;
	}
};

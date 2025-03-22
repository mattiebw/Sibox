#include "siboxpch.h"
#include "World/ChunkProvider.h"

#include "World/TileSets.h"

u16 DefaultChunkProvider::GetTileAt(int x, int y) const
{
	return TileSets::Grass;
}

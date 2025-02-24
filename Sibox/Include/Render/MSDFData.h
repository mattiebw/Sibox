#pragma once

#include "FontGeometry.h"
#include "GlyphGeometry.h"

// MW @todo: All we need to store after loading is the atlas texture and the font metrics
// We should use a custom type to store the metrics, to untie us from the msdf libraries, and completely avoid linking
// to them in a distribution build.
struct MSDFData
{
	std::vector<msdf_atlas::GlyphGeometry> Glyphs;
	msdf_atlas::FontGeometry               FontGeo;
};

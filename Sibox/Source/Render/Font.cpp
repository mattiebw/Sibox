#include "siboxpch.h"
#include "Render/Font.h"

#include <msdf-atlas-gen.h>
#include <msdfgen.h>

#include <algorithm>
#include <SDL3/SDL_gpu.h>

#include "Core/Application.h"
#include "Render/MSDFData.h"

msdfgen::FreetypeHandle *Font::s_FTHandle    = nullptr;
Ref<Font>                Font::s_DefaultFont = nullptr;

struct CharRange
{
	u32 Begin, End;
};

struct AtlasConfig
{
	int Width, Height;
};

u8 threadCount = std::thread::hardware_concurrency() / 2;

template <typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
static Ref<Texture> GenerateAtlasTexture(const std::string &                           fontName, f32 size,
                                         const std::vector<msdf_atlas::GlyphGeometry> &glpyhs,
                                         const msdf_atlas::FontGeometry &              geo, const AtlasConfig &config)
{
	// Set up some values we'll need later..
	TextureSpecification spec;
	spec.Width  = static_cast<s32>(config.Width);
	spec.Height = static_cast<s32>(config.Height);
	spec.Format = TextureFormat::RGB8;

	// First, lets see if it's cached.
	std::string cacheName = fmt::format("Cache/{}.atlas", fontName);
	Buffer      data      = Application::GetSavedDataManager().GetBinaryData(cacheName);
	if (data.Data)
	{
		// Okay, we already have all the data.
		Ref<Texture> texture = CreateRef<Texture>(spec);
		texture->SetData(data.Data);
		delete[] data.Data;
		return texture;
	}

	// Lets set up our atlas generator
	msdf_atlas::GeneratorAttributes attributes;
	attributes.scanlinePass          = true;
	attributes.config.overlapSupport = true;

	msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(
		config.Width, config.Height);
	generator.setAttributes(attributes);
	generator.setThreadCount(threadCount);
	// MW @todo: This should be configurable, or based on the number of cores. std::thread::hardware_concurrency()?

	// Generate the atlas!
	generator.generate(glpyhs.data(), static_cast<int>(glpyhs.size()));

	// Lets get our bitmap.
	msdfgen::BitmapConstRef<T, N> bitmap = static_cast<msdfgen::BitmapConstRef<T, N>>(generator.atlasStorage());

	// And set up a GPU texture for it:
	Ref<Texture> texture = CreateRef<Texture>(spec);
	texture->SetData(bitmap.pixels);

	// Let's also cache the texture to disk.
	// MW @hack: Technically the texture could have a different size due to some data manipulation by the Texture class.
	Application::GetSavedDataManager().SaveBinaryData(cacheName, {
		                                                  // ReSharper disable once CppCStyleCast
		                                                  (u8*)bitmap.pixels, texture->GetDataSize()
	                                                  });

	return texture;
}

Font::Font(const std::filesystem::path &fontPath)
{
	SIBOX_ASSERT(s_FTHandle && "Font system not initialized!");
	Stopwatch sw;

	// For now, we're just loading ttf files from disk.
	// In a larger project, we wouldn't have loose files or hardcoded paths, so we'd be loading it from a memory buffer
	// provided by an asset manager.
	// Even further, we'd probably cache the MSDF texture into an asset for a distribution build, at which point
	// loading the font would be a simple matter of loading the texture and the font metrics.

	std::string pathString = fontPath.string(); // Convert from path->string, as we need a c_str, but not wide!

	msdfgen::FontHandle *font = loadFont(s_FTHandle, pathString.c_str());

	if (!font)
	{
		SIBOX_ERROR("Failed to load font from path: {0}", pathString);
		return;
	}

	m_Data          = CreateScope<MSDFData>();
	m_Data->FontGeo = msdf_atlas::FontGeometry(&m_Data->Glyphs);

	// Now lets add our charset.
	// MW @todo: This should be configurable per font.
	static constexpr CharRange charRanges[] = {{0x0020, 0x00FF}};
	msdf_atlas::Charset        charset;
	for (const auto [Begin, End] : charRanges)
		for (u32 i = Begin; i <= End; i++)
			charset.add(i);

	// Got the charset, lets load it!
	m_GlyphCount = m_Data->FontGeo.loadCharset(font, 1.0, charset);

	// Now lets pack our atlas.
	// First, set up our packer:
	// MW @todo: The following settings should probably be configurable. Also I don't know what they do!
	msdf_atlas::TightAtlasPacker packer;
	packer.setPixelRange(2.0);
	packer.setMiterLimit(1.0);
	packer.setInnerPixelPadding(0);
	packer.setOuterPixelPadding(0);
	packer.setInnerUnitPadding(0);
	packer.setOuterUnitPadding(0);
	f64 emSize = 40.0;
	packer.setScale(emSize);

	// And do the packing
	int remaining = packer.pack(m_Data->Glyphs.data(), static_cast<int>(m_Data->Glyphs.size()));
	SIBOX_ASSERT(remaining == 0 && "Failed to pack atlas!");
	int width, height;
	packer.getDimensions(width, height);
	emSize = packer.getScale();

	// And our edge colouring, for MSDF
	constexpr u64  lcgMultiplier      = 6364136223846793005ull;
	constexpr u64  lcgIncrement       = 1442695040888963407ull;
	constexpr u64  colouringSeed      = 0;
	constexpr bool expensiveColouring = true;

	if (expensiveColouring)
	{
		msdf_atlas::Workload([&glyphs = m_Data->Glyphs](int i, int threadNo) -> bool
		{
			// Copied from msdf-atlas-gen!
			u64 glyphSeed = (lcgMultiplier * (colouringSeed ^ i) + lcgIncrement) * !!false;
			glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, 3.0, glyphSeed);
			return true;
		}, static_cast<int>(m_Data->Glyphs.size())).finish(threadCount);
	}
	else
	{
		u64 glyphSeed = colouringSeed;
		for (auto &glyph : m_Data->Glyphs)
		{
			glyphSeed *= lcgMultiplier;
			glyph.edgeColoring(msdfgen::edgeColoringInkTrap, 3.0, glyphSeed);
		}
	}

	// Now let's generate the atlas texture!
	m_Texture = GenerateAtlasTexture<u8, f32, 3, msdf_atlas::msdfGenerator>(
		fontPath.filename().generic_string(), static_cast<f32>(emSize), m_Data->Glyphs, m_Data->FontGeo,
		{width, height});

	// Clean up!
	destroyFont(font);

	sw.End();
	SIBOX_TRACE("Loaded font ({0}/{1} glyphs) from file \"{2}\" in {3}ms", m_GlyphCount, charset.size(), pathString,
	            sw.GetElapsedMilliseconds());
}

Font::~Font()
{
}

void Font::InitFontSystem()
{
	s_FTHandle = msdfgen::initializeFreetype();
	if (!s_FTHandle)
	{
		SIBOX_ERROR("Failed to initialize FreeType!");
		return;
	}

	s_DefaultFont = CreateRef<Font>("Content/Fonts/OpenSans-Regular.ttf");
}

void Font::ShutdownFontSystem()
{
	s_DefaultFont = nullptr;
	if (s_FTHandle)
		deinitializeFreetype(s_FTHandle);
	s_FTHandle = nullptr;
}

FontMeasurement Font::MeasureString(const std::string &string, const Vector3F &scale)
{
	msdfgen::FontMetrics metrics = m_Data->FontGeo.getMetrics();
	f32                  fsScale = 1.0f / static_cast<f32>(metrics.ascenderY - metrics.descenderY);
	Vector2F             pen(0, 0);

	f32 minX = 0, maxX = 0;
	f32 minY = 0, maxY = 0;

	for (int i = 0; i < string.size(); i++)
	{
		auto glyph = m_Data->FontGeo.getGlyph(string[i]);
		if (!glyph)
			glyph = m_Data->FontGeo.getGlyph('?');
		if (!glyph)
		{
			SIBOX_ASSERT(false && "Failed to draw string with font - missing char, and '?' char!");
			return {};
		}

		f64 quadLeft, quadBottom, quadRight, quadTop;
		glyph->getQuadPlaneBounds(quadLeft, quadBottom, quadRight, quadTop);
		Vector2F quadMin(static_cast<f32>(quadLeft) * fsScale, static_cast<f32>(quadBottom) * fsScale);
		Vector2F quadMax(static_cast<float>(quadRight) * fsScale, static_cast<float>(quadTop) * fsScale);
		quadMin += pen;
		quadMax += pen;

		minX = std::min(minX, quadMin.X);
		minY = std::min(minY, quadMin.Y);
		maxX = std::max(maxX, quadMax.X);
		maxY = std::max(maxY, quadMax.Y);

		f64 advance = glyph->getAdvance();
		if (i != string.size() - 1)
			m_Data->FontGeo.getAdvance(advance, string[i], string[i + 1]);
		pen.X += static_cast<float>(advance) * fsScale;
	}

	FontMeasurement measurement;
	measurement.Size   = {maxX - minX, maxY - minY};
	measurement.Offset = {minX, minY};
	measurement.Size.X *= scale.X;
	measurement.Size.Y *= scale.Y;
	measurement.Offset.X *= scale.X;
	measurement.Offset.Y *= scale.Y;
	return measurement;
}

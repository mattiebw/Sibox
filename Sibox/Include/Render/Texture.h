#pragma once

enum class WrapMode
{
	Repeat         = 0x2901, // GL_REPEAT
	MirroredRepeat = 0x8370, // GL_MIRRORED_REPEAT
	ClampToEdge    = 0x812F, // GL_CLAMP_TO_EDGE
	ClampToBorder  = 0x812D, // GL_CLAMP_TO_BORDER
};

enum class FilterMode
{
	Nearest = 0x2600, // GL_NEAREST
	Linear  = 0x2601, // GL_LINEAR
};

enum class TextureFormat
{
	RGBA8 = 0x8058, // GL_RGBA8
	RGB8  = 0x8051, // GL_RGB8
};

NODISCARD FORCEINLINE int ChannelsFromFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RGBA8:
		return 4;
	case TextureFormat::RGB8:
		return 3;
	default:
		SIBOX_ASSERT(false && "Invalid texture format");
		return 0;
	}
}

NODISCARD FORCEINLINE int FormatToGLFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RGBA8:
		return 0x1908; // GL_RGBA
	case TextureFormat::RGB8:
		return 0x1907; // GL_RGB
	default:
		SIBOX_ASSERT(false && "Invalid texture format");
		return 0;
	}
}

struct TextureSpecification
{
	bool          FlipVertically  = true;
	bool          GenerateMipmaps = true;
	TextureFormat Format          = TextureFormat::RGBA8;
	WrapMode      Wrap            = WrapMode::Repeat;
	FilterMode    MinFilter       = FilterMode::Linear;
	FilterMode    MagFilter       = FilterMode::Linear;
	int32_t       Width           = 0;
	int32_t       Height          = 0;
};

class Texture
{
public:
	explicit Texture(const TextureSpecification &spec);
	Texture(std::string_view filename, const TextureSpecification &spec = {});
	~Texture();

	void SetData(const uint8_t *data);
	void SetData(const Buffer &data);

	void Activate(int slot = 0) const;
	void CleanUp();

	NODISCARD FORCEINLINE int32_t  GetWidth() const { return m_Spec.Width; }
	NODISCARD FORCEINLINE int32_t  GetHeight() const { return m_Spec.Height; }
	NODISCARD FORCEINLINE int32_t  GetChannels() const { return ChannelsFromFormat(m_Spec.Format); }
	NODISCARD FORCEINLINE uint32_t GetOpenGLID() const { return m_TextureID; }
	// MW @todo: This depends on format.
	NODISCARD FORCEINLINE uint32_t GetDataSize() const { return m_Spec.Width * m_Spec.Height * GetChannels(); }
	NODISCARD FORCEINLINE const TextureSpecification& GetSpecification() const { return m_Spec; }

	NODISCARD FORCEINLINE bool IsValid() const { return m_TextureID != 0; }

	bool operator==(const Texture &other) const
	{
		return m_TextureID == other.GetOpenGLID();
	}

private:
	TextureSpecification m_Spec;
	uint32_t             m_TextureID = -1;
};

#pragma once

enum class FramebufferTextureFormat
{
	None = 0,
	RGBA8,
	RED_INTEGER,
	DEPTH24_STENCIL8,
	Depth = DEPTH24_STENCIL8,
};

uint32_t FramebufferTextureFormatToGLFormat(FramebufferTextureFormat format);
uint32_t FramebufferTextureFormatToGLType(FramebufferTextureFormat format);

struct FramebufferTexture
{
	FramebufferTexture() = default;

	FramebufferTexture(FramebufferTextureFormat format)
		: Format(format)
	{
	}

	FramebufferTextureFormat Format     = FramebufferTextureFormat::None;
	uint32_t                 RendererID = 0;
};

struct FramebufferSpecification
{
	glm::ivec2 Size;
	uint32_t   Samples         = 1;
	bool       SwapChainTarget = false;

	std::vector<FramebufferTexture> Attachments;
};

class Framebuffer
{
public:
	Framebuffer(FramebufferSpecification spec);
	~Framebuffer();

	void Invalidate();

	void Resize(uint32_t width, uint32_t height);
	void Resize(glm::ivec2 size);

	NODISCARD int32_t ReadPixel(uint32_t attachmentIndex, int x, int y);
	void              ClearColorAttachment(uint32_t attachmentIndex, int value);

	NODISCARD FORCEINLINE const FramebufferTexture& GetColorAttachment(uint32_t attachmentIndex) const
	{
		return m_Attachments[attachmentIndex];
	};
	NODISCARD FORCEINLINE const FramebufferTexture& GetDepthAttachment() const { return m_DepthAttachment; };

	void        Bind();
	static void Unbind();

	NODISCARD FORCEINLINE const FramebufferSpecification& GetSpecification() const { return m_Spec; }

private:
	uint32_t                        m_RendererID;
	std::vector<FramebufferTexture> m_Attachments;
	FramebufferTexture              m_DepthAttachment;
	FramebufferSpecification        m_Spec;
};

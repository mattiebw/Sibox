#pragma once

enum class FramebufferTextureFormat
{
	None = 0,
	RGBA8,
	RED_INTEGER,
	DEPTH24_STENCIL8,
	Depth = DEPTH24_STENCIL8,
};

u32 FramebufferTextureFormatToGLFormat(FramebufferTextureFormat format);
u32 FramebufferTextureFormatToGLType(FramebufferTextureFormat format);

struct FramebufferTexture
{
	FramebufferTexture() = default;

	FramebufferTexture(FramebufferTextureFormat format)
		: Format(format)
	{
	}

	FramebufferTextureFormat Format     = FramebufferTextureFormat::None;
	u32                 RendererID = 0;
};

struct FramebufferSpecification
{
	Vector2I Size;
	u32 Samples         = 1;
	bool     SwapChainTarget = false;

	std::vector<FramebufferTexture> Attachments;
};

class Framebuffer
{
public:
	Framebuffer(FramebufferSpecification spec);
	~Framebuffer();

	void Invalidate();

	void Resize(s32 width, s32 height);
	void Resize(Vector2I size);

	NODISCARD s32 ReadPixel(u32 attachmentIndex, int x, int y);
	void              ClearColorAttachment(u32 attachmentIndex, int value);

	NODISCARD FORCEINLINE const FramebufferTexture& GetColorAttachment(u32 attachmentIndex) const
	{
		return m_Attachments[attachmentIndex];
	};
	NODISCARD FORCEINLINE const FramebufferTexture& GetDepthAttachment() const { return m_DepthAttachment; };

	void        Bind();
	static void Unbind();

	NODISCARD FORCEINLINE const FramebufferSpecification& GetSpecification() const { return m_Spec; }

private:
	u32                        m_RendererID;
	std::vector<FramebufferTexture> m_Attachments;
	FramebufferTexture              m_DepthAttachment;
	FramebufferSpecification        m_Spec;
};

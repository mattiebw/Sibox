#include "siboxpch.h"
#include "Render/Framebuffer.h"

uint32_t FramebufferTextureFormatToGLFormat(FramebufferTextureFormat format)
{
	switch (format)
	{
	case FramebufferTextureFormat::RGBA8:
		return GL_RGBA8;
	case FramebufferTextureFormat::RED_INTEGER:
		return GL_RED_INTEGER;
	case FramebufferTextureFormat::DEPTH24_STENCIL8:
		return GL_DEPTH24_STENCIL8;
	}

	SIBOX_ERROR("FramebufferTextureFormatToGL: Unknown FramebufferTextureFormat");
	return 0;
}

uint32_t FramebufferTextureFormatToGLInternalFormat(FramebufferTextureFormat format)
{
	switch (format)
	{
	case FramebufferTextureFormat::RGBA8:
		return GL_RGBA8;
	case FramebufferTextureFormat::RED_INTEGER:
		return GL_R32I;
	}

	SIBOX_ERROR("FramebufferTextureFormatToGL: Unknown FramebufferTextureFormat");
	return 0;
}

uint32_t FramebufferTextureFormatToGLType(FramebufferTextureFormat format)
{
	switch (format)
	{
	case FramebufferTextureFormat::RGBA8:
		return GL_UNSIGNED_BYTE;
	case FramebufferTextureFormat::RED_INTEGER:
		return GL_INT;
	case FramebufferTextureFormat::DEPTH24_STENCIL8:
		return GL_UNSIGNED_INT;
	}

	SIBOX_ERROR("FramebufferTextureFormatToGL: Unknown FramebufferTextureFormat");
	return 0;
}

Framebuffer::Framebuffer(FramebufferSpecification spec)
	: m_Spec(std::move(spec))
{
	if (m_Spec.SwapChainTarget)
		return;

	for (auto attachmentSpec : m_Spec.Attachments)
	{
		if (attachmentSpec.Format == FramebufferTextureFormat::DEPTH24_STENCIL8)
			m_DepthAttachment = attachmentSpec;
		else
			m_Attachments.emplace_back(attachmentSpec);
	}
	Invalidate();
}

Framebuffer::~Framebuffer()
{
	if (m_Spec.SwapChainTarget)
		return;

	glDeleteFramebuffers(1, &m_RendererID);
	for (const auto &attachment : m_Attachments)
		glDeleteTextures(1, &attachment.RendererID);
	if (m_DepthAttachment.RendererID != 0)
		glDeleteTextures(1, &m_DepthAttachment.RendererID);
}

void Framebuffer::Invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);

		for (const auto &attachment : m_Attachments)
			glDeleteTextures(1, &attachment.RendererID);
		if (m_DepthAttachment.RendererID != 0)
			glDeleteTextures(1, &m_DepthAttachment.RendererID);

		m_Attachments.clear();
		m_DepthAttachment.RendererID = 0;
	}

	glCreateFramebuffers(1, &m_RendererID);
	Bind();

	bool   multisample = m_Spec.Samples > 1;
	GLenum target      = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	int i = 0;
	for (auto &attachment : m_Spec.Attachments)
	{
		glCreateTextures(target, 1, &attachment.RendererID);
		glBindTexture(target, attachment.RendererID);

		if (multisample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Spec.Samples,
			                        FramebufferTextureFormatToGLInternalFormat(attachment.Format), m_Spec.Size.x,
			                        m_Spec.Size.y, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, FramebufferTextureFormatToGLInternalFormat(attachment.Format), m_Spec.Size.x,
			             m_Spec.Size.y, 0, FramebufferTextureFormatToGLFormat(attachment.Format), GL_UNSIGNED_BYTE,
			             nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, target, attachment.RendererID, 0);
	}

	if (m_DepthAttachment.Format != FramebufferTextureFormat::None)
	{
		glCreateTextures(target, 1, &m_DepthAttachment.RendererID);
		glBindTexture(target, m_DepthAttachment.RendererID);
		if (multisample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Spec.Samples,
			                        FramebufferTextureFormatToGLInternalFormat(m_DepthAttachment.Format), m_Spec.Size.x,
			                        m_Spec.Size.y, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, FramebufferTextureFormatToGLInternalFormat(m_DepthAttachment.Format),
			               m_Spec.Size.x, m_Spec.Size.y);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, target, m_DepthAttachment.RendererID, 0);
	}

	if (m_Attachments.size() > 1)
	{
		SIBOX_ASSERT(m_Attachments.size() < 4 && "Maximum of 4 attachments");
		GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(static_cast<GLsizei>(m_Attachments.size()), buffers);
	}
	else
	{
		glDrawBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SIBOX_ERROR("Framebuffer is not complete");
	}

	Unbind();
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
	{
		SIBOX_ERROR("Framebuffer::Resize: Invalid width/height");
		return;
	}

	m_Spec.Size = {width, height};
	Invalidate();
}

void Framebuffer::Resize(glm::ivec2 size)
{
	Resize(size.x, size.y);
}

int32_t Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
{
	SIBOX_ASSERT(attachmentIndex < m_Attachments.size() && "Invalid attachment index");

	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int  pixelData;
	auto format = m_Attachments[attachmentIndex].Format;
	glReadPixels(x, y, 1, 1, FramebufferTextureFormatToGLFormat(format),
	             FramebufferTextureFormatToGLType(format), &pixelData);
	return pixelData;
}

void Framebuffer::ClearColorAttachment(uint32_t attachmentIndex, int value)
{
	SIBOX_ASSERT(attachmentIndex < m_Attachments.size() && "Invalid attachment index");

	auto &attachment = m_Attachments[attachmentIndex];
	glClearTexImage(attachment.RendererID, attachmentIndex, 1, FramebufferTextureFormatToGLFormat(attachment.Format),
	                &value);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Spec.SwapChainTarget ? 0 : m_RendererID);
	glViewport(0, 0, m_Spec.Size.x, m_Spec.Size.y);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

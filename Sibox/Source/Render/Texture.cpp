#include "papipch.h"
#include "Render/Texture.h"

#include <stb_image.h>

Texture::Texture(const TextureSpecification &spec)
	: m_Spec(spec)
{
	// Create OpenGL texture
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	// Setup texture storage parameters
	glTextureStorage2D(m_TextureID, 1, static_cast<GLenum>(m_Spec.Format), m_Spec.Width, m_Spec.Height);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_Spec.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_Spec.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_Spec.MinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_Spec.MagFilter));
}

Texture::Texture(std::string_view filename, const TextureSpecification &spec)
	: m_Spec(spec)
{
	// Load texture from file using stb_image
	PAPI_TRACE("Loading texture from file \"{0}\"", filename);
	stbi_set_flip_vertically_on_load(m_Spec.FlipVertically);
	uint8_t *data = stbi_load(filename.data(), &m_Spec.Width, &m_Spec.Height, nullptr, GetChannels());

	// Error check texture loading
	if (data == nullptr)
	{
		PAPI_ERROR("Failed to load texture \"{0}\": {1}", filename, stbi_failure_reason());
		return; // TODO: Not properly indicating failure - best we do is 0 texture ID and 0 width/height
	}

	// Create OpenGL texture
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	// Setup texture storage parameters
	glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(m_Spec.Format), m_Spec.Width, m_Spec.Height);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_Spec.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_Spec.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_Spec.MinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_Spec.MagFilter));

	// Set data
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Spec.Width, m_Spec.Height, FormatToGLFormat(m_Spec.Format),
	                GL_UNSIGNED_BYTE, data);

	// Generate mipmaps if needed
	if (m_Spec.GenerateMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::CleanUp()
{
	PAPI_TRACE("Cleaning up texture {0}", m_TextureID);
	glDeleteTextures(1, &m_TextureID);
	m_TextureID = 0;
}

Texture::~Texture()
{
	CleanUp();
}

void Texture::SetData(const uint8_t *data)
{
	// MW @todo: Error checking of size. Either take a size parameter, or remove this overload and require a Buffer.
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Spec.Width, m_Spec.Height, FormatToGLFormat(m_Spec.Format),
	                GL_UNSIGNED_BYTE, data);
}

void Texture::SetData(const Buffer &data)
{
	PAPI_ASSERT(data.Size == GetDataSize() && "Buffer size doesn't match texture size.");
	SetData(data.Data);
}

void Texture::Activate(int slot) const
{
	PAPI_ASSERT(m_TextureID != 0 && "Texture activated when not initialized");

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

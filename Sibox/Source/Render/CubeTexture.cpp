#include "siboxpch.h"
#include "Render/CubeTexture.h"

#include <stb_image.h>

CubeTexture::CubeTexture(std::vector<std::string_view> filenames, const TextureSpecification &spec)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    int width, height, numberChannels;
	stbi_set_flip_vertically_on_load(m_Spec.FlipVertically);
    for (s32 i = 0; i < filenames.size(); i++)
    {
        void* data = stbi_load(filenames[i].data(), &m_Spec.Width, &m_Spec.Height, nullptr, GetChannels());
        
        // Error check texture loading
        if (data == nullptr)
        {
            SIBOX_ERROR("Failed to load texture \"{0}\": {1}", filenames[i].data(), stbi_failure_reason());
            return; // TODO: Not properly indicating failure - best we do is 0 texture ID and 0 width/height
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, static_cast<GLenum>(m_Spec.Format), m_Spec.Width, m_Spec.Height, 0, FormatToGLFormat(m_Spec.Format), GL_UNSIGNED_BYTE, data);

	    stbi_image_free(data);
    }
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_Spec.Wrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_Spec.Wrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(m_Spec.Wrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_Spec.MinFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_Spec.MagFilter));
}

CubeTexture::~CubeTexture()
{
    CleanUp();
}

void CubeTexture::Activate(int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

void CubeTexture::CleanUp()
{
    SIBOX_TRACE("Cleaning up cube texture {0}", m_TextureID);
    glDeleteTextures(1, &m_TextureID);
    m_TextureID = 0;
}

#pragma once

#include "Render/Texture.h"

class CubeTexture
{
public:
    CubeTexture(std::vector<std::string_view> filenames, const TextureSpecification &spec = {});
    ~CubeTexture();

    // MW @todo: These should be implemented.
    CubeTexture(const Texture &)            = delete;
    CubeTexture& operator=(const CubeTexture &) = delete;
    CubeTexture(Texture &&)                 = delete;
    CubeTexture& operator=(CubeTexture &&)      = delete;

    void Activate(int slot = 0) const;
    void CleanUp();

    NODISCARD FORCEINLINE s32 GetWidth() const { return m_Spec.Width; }
    NODISCARD FORCEINLINE s32 GetHeight() const { return m_Spec.Height; }
    NODISCARD FORCEINLINE s32 GetChannels() const { return ChannelsFromFormat(m_Spec.Format); }
    NODISCARD FORCEINLINE u32 GetOpenGLID() const { return m_TextureID; }
    // MW @todo: This depends on format.
    NODISCARD FORCEINLINE u32 GetDataSize() const { return m_Spec.Width * m_Spec.Height * GetChannels(); }
    NODISCARD FORCEINLINE const TextureSpecification& GetSpecification() const { return m_Spec; }

    NODISCARD FORCEINLINE bool IsValid() const { return m_TextureID != 0; }

    bool operator==(const Texture &other) const
    {
        return m_TextureID == other.GetOpenGLID();
    }

private:
    TextureSpecification m_Spec;
    u32                  m_TextureID = -1;
};


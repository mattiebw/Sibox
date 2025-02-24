#pragma once

#include <SDL3/SDL_events.h>

#include "RenderBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class Font;
class TileMapChunk;
class Texture;
class BufferLayout;
class Viewport;
class Window;

struct RendererSpecification
{
	bool VSync = false;
};

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float     TexIndex;

	static BufferLayout GetLayout()
	{
		return std::move(BufferLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Color", ShaderDataType::Float4},
			{"a_TexCoord", ShaderDataType::Float2},
			{"a_TexIndex", ShaderDataType::Float},
		}));
	}
};

struct TileVertex
{
	glm::vec2 Position;
};

struct TextVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float     FontAtlas;

	static BufferLayout GetLayout()
	{
		return std::move(BufferLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Color", ShaderDataType::Float4},
			{"a_TexCoord", ShaderDataType::Float2},
			{"a_FontAtlas", ShaderDataType::Float},
		}));
	}
};

class TextureSet
{
public:
	void SetMaxSlots(int max);

	void BindTextures() const;
	void Reset();
	bool HasTexture(const Ref<Texture> &texture, int &index);
	int  FindOrAddTexture(const Ref<Texture> &texture);

	MulticastDelegate<> OnFlush;

protected:
	int                       m_MaxSlots = 0;
	std::vector<Ref<Texture>> m_TextureSlots;
	size_t                    m_TextureSlotIndex = 0;
};

class QuadBatch
{
public:
	QuadBatch(class RendererData *data, uint32_t MaxQuads = 10000);
	~QuadBatch();

	void DrawQuad(const glm::mat4 &transform, const glm::vec4 &     tintColor, const glm::vec2 &texCoordMin,
	              const glm::vec2 &texCoordMax, const Ref<Texture> &texture);
	FORCEINLINE void DrawQuad(const glm::mat4 &transform, const glm::vec4 &tintColor, const Ref<Texture> &texture)
	{
		DrawQuad(transform, tintColor, {0.0f, 0.0f}, {1.0f, 1.0f}, texture);
	}

	void DrawQuad(const glm::mat4 &transform, const glm::vec4 &tintColor);

	void DrawQuad(const glm::vec3 &centerPosition, const glm::vec2 &size, const glm::vec2 &        texCoordMin,
	              const glm::vec2 &texCoordMax, const glm::vec4 &   tintColor, const Ref<Texture> &texture);
	FORCEINLINE void DrawQuad(const glm::vec3 &   position, const glm::vec2 &size, const glm::vec4 &tintColor,
	                          const Ref<Texture> &texture)
	{
		DrawQuad(position, size, {0.0f, 0.0f}, {1.0f, 1.0f}, tintColor, texture);
	}

	void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &tintColor);

	void DrawRectangle(const FRect& rect, const glm::vec4& colour);
	void DrawRectangleLines(const FRect& rect, const glm::vec4& colour, float thickness);

	void Flush();
	void Reset();

private:
	RendererData *m_Data = nullptr;
	uint32_t      m_MaxQuads, m_MaxVertices, m_MaxIndices;
	glm::vec4     m_QuadPositions[4];
	glm::vec3     m_QuadPositions3[4];

	TextureSet        m_Textures;
	uint32_t          m_IndicesCount = 0;
	Ref<Shader>       m_Shader;
	Ref<VertexArray>  m_VertexArray;
	Ref<VertexBuffer> m_VertexBuffer;
	QuadVertex *      m_VertexBufferBase = nullptr;
	QuadVertex *      m_VertexBufferPtr  = nullptr;
};

class TilemapRenderer
{
public:
	void Init(RendererData *data);

	void DrawTileMapChunk(glm::vec3 bottomLeftPosition, TileMapChunk *chunk) const;

private:
	RendererData *m_Data = nullptr;
	Ref<Shader>   m_TilemapShader;
};

struct RenderStats
{
	uint32_t DrawCalls = 0;
	uint32_t QuadCount = 0;
	uint32_t TileCount = 0;
	uint32_t CharCount = 0;

	void Reset();
};

class TextRenderer
{
public:
	~TextRenderer();

	FORCEINLINE void Init(RendererData *data, uint32_t maxQuads = 10000);
	void             Flush();
	void             Reset();

	void DrawString(const std::string &string, Ref<Font> font, const glm::mat4 &transformation,
	                const glm::vec4 &  colour);

private:
	Ref<VertexArray>  m_VertexArray;
	Ref<VertexBuffer> m_VertexBuffer;

	TextureSet    m_Textures;
	Ref<Shader>   m_TextShader;
	RendererData *m_Data = nullptr;

	TextVertex *m_VertexPtr     = nullptr;
	TextVertex *m_VertexPtrBase = nullptr;
	int32_t     m_IndicesCount  = 0, m_MaxIndices = 0;
};

class RendererData
{
public:
	Ref<Texture> WhiteTexture;

	int MaxTextureSlots;

	RenderStats Stats;
};

class Renderer
{
public:
	Renderer(RendererSpecification rendererSpecification);
	~Renderer();

	NODISCARD static FORCEINLINE Viewport* GetCurrentViewport() { return s_CurrentViewport; }

	bool Init(Ref<Window> window);
	void Shutdown();

	void ProcessSDLEvent(const SDL_Event *e);
	void BeginFrame();
	void Render();
	void EndFrame();

	Ref<Viewport> CreateViewport();
	void          RemoveViewport(const Ref<Viewport> &viewport);

	NODISCARD FORCEINLINE const Ref<VertexBuffer>& GetTileQuadVertexBuffer() { return m_TileQuadVertexBuffer; }
	NODISCARD FORCEINLINE const Ref<IndexBuffer>&  GetTileQuadIndexBuffer() { return m_TileQuadIndexBuffer; }

	NODISCARD FORCEINLINE const Ref<QuadBatch>& GetQuadRenderer() const { return m_QuadBatch; }
	NODISCARD FORCEINLINE TilemapRenderer&      GetTilemapRenderer() { return m_TilemapRenderer; }
	NODISCARD FORCEINLINE TextRenderer&         GetTextRenderer() { return m_TextRenderer; }

	void SetVSync(bool enabled);

	static void GLErrorCallback(GLenum        source,
	                            GLenum        type,
	                            GLuint        id,
	                            GLenum        severity,
	                            GLsizei       length,
	                            const GLchar *message,
	                            const void *  userParam);

	MulticastDelegate<> DebugUIRenderCallback;

private:
	bool InitOpenGL();
	bool InitImGUI();
	bool OnWindowResize(Window *window, const glm::ivec2 &size);

	void RenderImGUI();

	void ShutdownImGUI();

	TilemapRenderer            m_TilemapRenderer;
	TextRenderer               m_TextRenderer;
	RendererSpecification      m_Specification;
	RendererData *             m_Data;
	Ref<QuadBatch>             m_QuadBatch;
	Ref<Window>                m_Window = nullptr;
	std::vector<Ref<Viewport>> m_Viewports;
	struct SDL_GLContextState *m_Context = nullptr; // SDL_GLContext.

	Ref<VertexBuffer> m_TileQuadVertexBuffer = nullptr;
	Ref<IndexBuffer>  m_TileQuadIndexBuffer  = nullptr;

	bool m_Initialised      = false;
	bool m_ImGUIInitialised = false;

	// Debug UI stuff
	bool m_DebugUIVisible = false;

	static Viewport *s_CurrentViewport;
};

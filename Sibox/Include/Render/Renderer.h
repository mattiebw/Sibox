#pragma once

#include <SDL3/SDL_events.h>

#include "Mesh.h"
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
	Vector3F Position;
	Vector4F Color;
	Vector2F TexCoord;
	f32      TexIndex;

	static BufferLayout GetLayout()
	{
		return BufferLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Color", ShaderDataType::Float4},
			{"a_TexCoord", ShaderDataType::Float2},
			{"a_TexIndex", ShaderDataType::Float},
		});
	}
};

struct TileVertex
{
	Vector2F Position;
};

struct TextVertex
{
	Vector3F Position;
	Vector4F Color;
	Vector2F TexCoord;
	f32      FontAtlas;

	static BufferLayout GetLayout()
	{
		return BufferLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Color", ShaderDataType::Float4},
			{"a_TexCoord", ShaderDataType::Float2},
			{"a_FontAtlas", ShaderDataType::Float},
		});
	}
};

class TextureSet
{
public:
	void SetMaxSlots(s32 max);

	void BindTextures() const;
	void Reset();
	bool HasTexture(const Ref<Texture> &texture, s32 &index) const;
	s32  FindOrAddTexture(const Ref<Texture> &texture);

	MulticastDelegate<> OnFlush;

protected:
	s32                       m_MaxSlots = 0;
	std::vector<Ref<Texture>> m_TextureSlots;
	s32                       m_TextureSlotIndex = 0;
};

class QuadBatch
{
public:
	QuadBatch(class RendererData *data, u32 MaxQuads = 10000);
	~QuadBatch();

	QuadBatch(const QuadBatch &)            = delete;
	QuadBatch& operator=(const QuadBatch &) = delete;
	QuadBatch(QuadBatch &&)                 = delete;
	QuadBatch& operator=(QuadBatch &&)      = delete;

	void DrawQuad(const Matrix4x4F &transform, const Vector4F &      tintColor, const Vector2F &texCoordMin,
	              const Vector2F &  texCoordMax, const Ref<Texture> &texture);
	FORCEINLINE void DrawQuad(const Matrix4x4F &transform, const Vector4F &tintColor, const Ref<Texture> &texture)
	{
		DrawQuad(transform, tintColor, {0.0f, 0.0f}, {1.0f, 1.0f}, texture);
	}

	void DrawQuad(const Matrix4x4F &transform, const Vector4F &tintColor);

	void DrawQuad(const Vector3F &centerPosition, const Vector2F &size, const Vector2F &         texCoordMin,
	              const Vector2F &texCoordMax, const Vector4F &   tintColor, const Ref<Texture> &texture);
	FORCEINLINE void DrawQuad(const Vector3F &    position, const Vector2F &size, const Vector4F &tintColor,
	                          const Ref<Texture> &texture)
	{
		DrawQuad(position, size, {0.0f, 0.0f}, {1.0f, 1.0f}, tintColor, texture);
	}

	void DrawQuad(const Vector3F &position, Vector2F size, const Vector4F &tintColor);

	void DrawRectangle(const RectF &rect, const Vector4F &colour);
	void DrawRectangleLines(const RectF &rect, const Vector4F &colour, f32 thickness);

	void Flush();
	void Reset();

private:
	RendererData *m_Data = nullptr;
	u32           m_MaxQuads, m_MaxVertices, m_MaxIndices;
	Vector4F      m_QuadPositions[4];
	Vector3F      m_QuadPositions3[4];

	TextureSet        m_Textures;
	u32               m_IndicesCount = 0;
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

	void DrawTileMapChunk(Vector3F bottomLeftPosition, TileMapChunk *chunk) const;

private:
	RendererData *m_Data = nullptr;
	Ref<Shader>   m_TilemapShader;
};

struct RenderStats
{
	u32 DrawCalls       = 0;
	u32 QuadCount       = 0;
	u32 TileCount       = 0;
	u32 CharCount       = 0;
	u32 MeshCount       = 0;
	u32 MeshVertexCount = 0;

	void Reset();
};

class TextRenderer
{
public:
	TextRenderer() = default;
	~TextRenderer();
	TextRenderer(const TextRenderer &)            = delete;
	TextRenderer& operator=(const TextRenderer &) = delete;
	TextRenderer(TextRenderer &&)                 = delete;
	TextRenderer& operator=(TextRenderer &&)      = delete;

	FORCEINLINE void Init(RendererData *data, u32 maxQuads = 10000);
	void             Flush();
	void             Reset();

	void DrawString(const std::string &string, Ref<Font> font, const Matrix4x4F &transformation,
	                const Vector4F &   colour);

private:
	Ref<VertexArray>  m_VertexArray;
	Ref<VertexBuffer> m_VertexBuffer;

	TextureSet    m_Textures;
	Ref<Shader>   m_TextShader;
	RendererData *m_Data = nullptr;

	TextVertex *m_VertexPtr     = nullptr;
	TextVertex *m_VertexPtrBase = nullptr;
	s32         m_IndicesCount  = 0, m_MaxIndices = 0;
};

class RendererData
{
public:
	Ref<Texture> WhiteTexture;

	s32 MaxTextureSlots;

	RenderStats Stats;
};

class Renderer
{
public:
	Renderer(RendererSpecification rendererSpecification);
	~Renderer();

	Renderer(const Renderer &)            = delete;
	Renderer& operator=(const Renderer &) = delete;
	Renderer(Renderer &&)                 = delete;
	Renderer& operator=(Renderer &&)      = delete;

	NODISCARD static FORCEINLINE Viewport* GetCurrentViewport() { return s_CurrentViewport; }

	bool Init(Ref<Window> window);
	void Shutdown();

	void ProcessSDLEvent(const SDL_Event *e);
	void BeginFrame();
	void Render();
	void EndFrame();

	Ref<Viewport> CreateViewport();
	void          RemoveViewport(const Ref<Viewport> &viewport);

	void DrawMesh(Mesh *mesh, const Matrix4x4F &transform) const;

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
	bool OnWindowResize(Window *window, Vector2I size);

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

	Ref<Shader> m_MeshShader;

	bool m_Initialised      = false;
	bool m_ImGUIInitialised = false;

	// Debug UI stuff
	bool m_DebugUIVisible = false;

	static Viewport *s_CurrentViewport;
};

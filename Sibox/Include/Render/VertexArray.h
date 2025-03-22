#pragma once
#include "RenderBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void        Bind();
	static void Unbind();

	void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer);
	void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer);

	NODISCARD FORCEINLINE const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
	NODISCARD FORCEINLINE const Ref<IndexBuffer>&               GetIndexBuffer() const { return m_IndexBuffer; }

private:
	u32                            m_RendererID;
	u32                            m_VertexBufferIndex = 0;
	std::vector<Ref<VertexBuffer>> m_VertexBuffers;
	Ref<IndexBuffer>               m_IndexBuffer;
};

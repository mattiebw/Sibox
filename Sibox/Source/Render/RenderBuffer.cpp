#include "siboxpch.h"
#include "Render/RenderBuffer.h"

#include "Render/Renderer.h"

u32 BufferUsageTypeToGL(BufferUsageType e)
{
	switch (e)
	{
	case BufferUsageType::StreamDraw: return GL_STREAM_DRAW;
	case BufferUsageType::StreamRead: return GL_STREAM_READ;
	case BufferUsageType::StreamCopy: return GL_STREAM_COPY;
	case BufferUsageType::StaticDraw: return GL_STATIC_DRAW;
	case BufferUsageType::StaticRead: return GL_STATIC_READ;
	case BufferUsageType::StaticCopy: return GL_STATIC_COPY;
	case BufferUsageType::DynamicDraw: return GL_DYNAMIC_DRAW;
	case BufferUsageType::DynamicRead: return GL_DYNAMIC_READ;
	case BufferUsageType::DynamicCopy: return GL_DYNAMIC_COPY;
	case BufferUsageType::None:
	default:
		SIBOX_ASSERT(false && "Unkown BufferUsageType");
		return 0;
	}
}

void BufferLayout::CalculateOffsetsAndStride()
{
	size_t offset = 0;
	m_Stride      = 0;

	for (auto &element : m_Elements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}
}

VertexBuffer::VertexBuffer() = default;

bool VertexBuffer::Create(u32 size, BufferUsageType type)
{
	glCreateBuffers(1, &m_RendererID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, BufferUsageTypeToGL(type));

	return true;
}

bool VertexBuffer::Create(const void *data, u32 size, BufferUsageType type)
{
	glCreateBuffers(1, &m_RendererID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageTypeToGL(type));

	return true;
}

bool VertexBuffer::Create(const Buffer &buffer, BufferUsageType type)
{
	return Create(buffer.Data, static_cast<u32>(buffer.Size), type);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const void *data, u32 size)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::SetData(const Buffer &buffer)
{
	SetData(buffer.Data, static_cast<u32>(buffer.Size));
}

IndexBuffer::~IndexBuffer()
{
	Destroy();
}

bool IndexBuffer::Create(const void *indices, u32 count, IndexType indexType, const BufferUsageType type)
{
	m_Count = count;
	glCreateBuffers(1, &m_RendererID);
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(u32), indices, BufferUsageTypeToGL(type));
	m_IndexType = indexType;
	return true;
}

bool IndexBuffer::Create(const Buffer &buffer, IndexType indexType, BufferUsageType type)
{
	return Create(reinterpret_cast<u32*>(buffer.Data), static_cast<u32>(buffer.Size / 4), indexType, type);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Destroy()
{
	if (m_RendererID != 0)
		glDeleteBuffers(1, &m_RendererID);
	m_RendererID = 0;
	m_Count      = 0;
}

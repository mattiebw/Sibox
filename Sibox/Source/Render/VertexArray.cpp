#include "siboxpch.h"
#include "Render/VertexArray.h"

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

VertexArray::VertexArray(VertexArray &&other) noexcept
{
	m_RendererID = other.m_RendererID;
	m_VertexBufferIndex = other.m_VertexBufferIndex;
	m_VertexBuffers      = std::move(other.m_VertexBuffers);
	m_IndexBuffer        = std::move(other.m_IndexBuffer);
	
	other.m_RendererID = 0;
	other.m_VertexBufferIndex = 0;
	other.m_VertexBuffers.clear();
	other.m_IndexBuffer = nullptr;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
{
	SIBOX_ASSERT(vertexBuffer->GetLayout().HasElements() && "Vertex Buffer must have a layout.");
	Bind();
	vertexBuffer->Bind();

	const auto &layout = vertexBuffer->GetLayout();
	for (const auto &element : layout)
	{
		switch (element.Type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
				                      ShaderDataTypeToGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
				                      layout.GetStride(), reinterpret_cast<const void*>(element.Offset));
				if (element.InstancingDivisor != 0)
					glVertexAttribDivisor(m_VertexBufferIndex, element.InstancingDivisor);
				m_VertexBufferIndex++;
			}
			break;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			{
				u8 count = element.GetComponentCount();
				for (u8 i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex, count, ShaderDataTypeToGLBaseType(element.Type),
					                      element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
					                      reinterpret_cast<const void*>(element.Offset + (sizeof(f32) * count * i)));
					if (element.InstancingDivisor != 0)
						glVertexAttribDivisor(m_VertexBufferIndex, element.InstancingDivisor);
					m_VertexBufferIndex++;
				}
			}
			break;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(),
				                       ShaderDataTypeToGLBaseType(element.Type), layout.GetStride(),
				                       reinterpret_cast<const void*>(element.Offset));
				if (element.InstancingDivisor != 0)
					glVertexAttribDivisor(m_VertexBufferIndex, element.InstancingDivisor);
				m_VertexBufferIndex++;
			}
			break;
		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
{
	Bind();
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}

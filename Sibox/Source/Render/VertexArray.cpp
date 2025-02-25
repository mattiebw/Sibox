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

void VertexArray::Bind()
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
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex, count, ShaderDataTypeToGLBaseType(element.Type),
					                      element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
					                      reinterpret_cast<const void*>(element.Offset + (sizeof(float) * count * i)));
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

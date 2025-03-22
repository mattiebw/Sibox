#pragma once

enum class BufferUsageType : u8
{
	None = 0,
	StreamDraw, StreamRead, StreamCopy,
	StaticDraw, StaticRead, StaticCopy,
	DynamicDraw, DynamicRead, DynamicCopy
};

NODISCARD FORCEINLINE const char* BufferUsageTypeToString(BufferUsageType e)
{
	switch (e)
	{
	case BufferUsageType::None: return "None";
	case BufferUsageType::StreamDraw: return "Stream Draw";
	case BufferUsageType::StreamRead: return "Stream Read";
	case BufferUsageType::StreamCopy: return "Stream Copy";
	case BufferUsageType::StaticDraw: return "Static Draw";
	case BufferUsageType::StaticRead: return "Static Read";
	case BufferUsageType::StaticCopy: return "Static Copy";
	case BufferUsageType::DynamicDraw: return "Dynamic Draw";
	case BufferUsageType::DynamicRead: return "Dynamic Read";
	case BufferUsageType::DynamicCopy: return "Dynamic Copy";
	default: return "Unknown";
	}
}

template <>
class fmt::formatter<BufferUsageType>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const BufferUsageType &bufferUsageType, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "{}", BufferUsageTypeToString(bufferUsageType));
	}
};

enum class ShaderDataType : u8 // MW @copypaste: Update below function(s) if enum extended.
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

NODISCARD FORCEINLINE const char* ShaderDataTypeToString(ShaderDataType e)
{
	switch (e)
	{
	case ShaderDataType::None: return "None";
	case ShaderDataType::Float: return "Float";
	case ShaderDataType::Float2: return "Float2";
	case ShaderDataType::Float3: return "Float3";
	case ShaderDataType::Float4: return "Float4";
	case ShaderDataType::Mat3: return "Mat3";
	case ShaderDataType::Mat4: return "Mat4";
	case ShaderDataType::Int: return "Int";
	case ShaderDataType::Int2: return "Int2";
	case ShaderDataType::Int3: return "Int3";
	case ShaderDataType::Int4: return "Int4";
	case ShaderDataType::Bool: return "Bool";
	default: return "Unknown";
	}
}

template <>
class fmt::formatter<ShaderDataType>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const ShaderDataType &shaderDataType, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "{}", ShaderDataTypeToString(shaderDataType));
	}
};

NODISCARD static u32 GetShaderDataTypeSize(ShaderDataType e)
{
	switch (e)
	{
	case ShaderDataType::Float: return 4 * 1;
	case ShaderDataType::Float2: return 4 * 2;
	case ShaderDataType::Float3: return 4 * 3;
	case ShaderDataType::Float4: return 4 * 4;
	case ShaderDataType::Mat3: return 4 * 3 * 3;
	case ShaderDataType::Mat4: return 4 * 4 * 4;
	case ShaderDataType::Int: return 4 * 1;
	case ShaderDataType::Int2: return 4 * 2;
	case ShaderDataType::Int3: return 4 * 3;
	case ShaderDataType::Int4: return 4 * 4;
	case ShaderDataType::Bool: return 1;
	case ShaderDataType::None:
	default:
		SIBOX_ASSERT(false && "Unknown shader data type.");
		return 0;
	}
}

NODISCARD static u32 GetShaderDataTypeComponentCount(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float: return 1;
	case ShaderDataType::Float2: return 2;
	case ShaderDataType::Float3: return 3;
	case ShaderDataType::Float4: return 4;
	case ShaderDataType::Mat3: return 3;
	case ShaderDataType::Mat4: return 4;
	case ShaderDataType::Int: return 1;
	case ShaderDataType::Int2: return 2;
	case ShaderDataType::Int3: return 3;
	case ShaderDataType::Int4: return 4;
	case ShaderDataType::Bool: return 1;
	case ShaderDataType::None:
	default:
		SIBOX_ASSERT(false && "Unknown shader data type.");
		return 0;
	}
}

NODISCARD static u32 ShaderDataTypeToGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
	case ShaderDataType::Mat3:
	case ShaderDataType::Mat4: return GL_FLOAT;
	case ShaderDataType::Int:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4: return GL_INT;
	case ShaderDataType::Bool: return GL_BOOL;
	case ShaderDataType::None:
	default:
		SIBOX_ASSERT(false && "Unknown shader data type.");
		return 0;
	}
}

NODISCARD static u32 BufferUsageTypeToGL(BufferUsageType e);

struct BufferElement
{
	std::string    Name;
	ShaderDataType Type;
	size_t         Offset;
	u32            Size;
	u32            InstancingDivisor;
	bool           Normalized;

	BufferElement() = default;

	BufferElement(std::string name, ShaderDataType type, size_t offset = 0, u32 instancingDivisor = 0,
	              bool        normalized                               = false)
		: Name(std::move(name)), Type(type), Offset(offset), Size(GetShaderDataTypeSize(type)),
		  InstancingDivisor(instancingDivisor), Normalized(normalized)
	{
	}

	NODISCARD FORCEINLINE u32 GetComponentCount() const { return GetShaderDataTypeComponentCount(Type); }
};

class BufferLayout
{
public:
	BufferLayout() = default;

	BufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	void AddElement(const BufferElement &element)
	{
		m_Elements.push_back(element);
		CalculateOffsetsAndStride();
	}

	NODISCARD FORCEINLINE u32                               GetStride() const { return m_Stride; }
	NODISCARD FORCEINLINE const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	NODISCARD FORCEINLINE bool                              HasElements() const { return m_Elements.size() > 0; }

	std::vector<BufferElement>::iterator       begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator       end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
	void CalculateOffsetsAndStride();

	std::vector<BufferElement> m_Elements;
	u32                        m_Stride = 0;
};

class VertexBuffer
{
public:
	VertexBuffer(u32 size, BufferUsageType type = BufferUsageType::StaticDraw);
	VertexBuffer(const void *data, u32 size, BufferUsageType type = BufferUsageType::StaticDraw);
	VertexBuffer(const Buffer &buffer, BufferUsageType type = BufferUsageType::StaticDraw);
	~VertexBuffer();

	VertexBuffer(VertexBuffer &&other)                 = delete; // MW @todo: Add other constructors
	VertexBuffer(const VertexBuffer &other)            = delete;
	VertexBuffer& operator=(const VertexBuffer &other) = delete;
	VertexBuffer& operator=(VertexBuffer &&other)      = delete;

	void        Bind() const;
	static void Unbind();

	void SetData(const void *data, u32 size);
	void SetData(const Buffer &buffer);

	NODISCARD FORCEINLINE const BufferLayout& GetLayout() const { return m_Layout; }
	void                                      SetLayout(const BufferLayout &layout) { m_Layout = layout; }

private:
	u32          m_RendererID;
	BufferLayout m_Layout;
};

class IndexBuffer
{
public:
	IndexBuffer(u32 *indices, u32 count, BufferUsageType type = BufferUsageType::StaticDraw);
	IndexBuffer(const Buffer &buffer, BufferUsageType type = BufferUsageType::StaticDraw);
	~IndexBuffer();

	IndexBuffer(IndexBuffer &&other)                 = delete; // MW @todo: Create other constructors
	IndexBuffer(const IndexBuffer &other)            = delete;
	IndexBuffer& operator=(const IndexBuffer &other) = delete;
	IndexBuffer& operator=(IndexBuffer &&other)      = delete;

	void        Bind();
	static void Unbind();

	NODISCARD FORCEINLINE u32 GetCount() const { return m_Count; }

private:
	u32 m_RendererID;
	u32 m_Count;
};

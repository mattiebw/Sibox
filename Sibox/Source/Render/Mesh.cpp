#include "siboxpch.h"
#include "Render/Mesh.h"

SubMesh::SubMesh() = default;

SubMesh::~SubMesh()
{
	delete[] m_Vertices;
	delete[] m_Indices;
}

SubMesh::SubMesh(SubMesh &&other) noexcept
{
	m_Vertices = other.m_Vertices;
	m_Indices   = other.m_Indices;
	m_NumVerts  = other.m_NumVerts;
	m_NumIndices = other.m_NumIndices;
	m_MaterialIndex = other.m_MaterialIndex;
	
	other.m_Vertices  = nullptr;
	other.m_Indices    = nullptr;
	other.m_NumVerts  = 0;
	other.m_NumIndices = 0;
	other.m_MaterialIndex = 0;
}

bool SubMesh::LoadFromBuffer(BufferReader &buffer)
{
	m_NumVerts = buffer.Read<u32>();
	m_NumIndices = buffer.Read<u32>();
	m_MaterialIndex = buffer.Read<u32>();

	m_Vertices = new MeshVertex[m_NumVerts];
	buffer.CopyTo(reinterpret_cast<u8*>(m_Vertices), sizeof(MeshVertex) * m_NumVerts);

	m_Indices = new u16[m_NumIndices];
	buffer.CopyTo(reinterpret_cast<u8*>(m_Indices), sizeof(u16) * m_NumIndices);

	Ref<VertexBuffer> vertexBuffer = CreateRef<VertexBuffer>();
	vertexBuffer->Create(m_Vertices, static_cast<u32>(sizeof(MeshVertex) * m_NumVerts));
	vertexBuffer->SetLayout(MeshVertex::GetLayout());

	Ref<IndexBuffer> indexBuffer = CreateRef<IndexBuffer>();
	indexBuffer->Create(m_Indices, m_NumIndices, IndexType::U16);

	m_VertexArray.AddVertexBuffer(vertexBuffer);
	m_VertexArray.SetIndexBuffer(indexBuffer);
	
	return true;
}

Mesh::Mesh(const MeshSpecification &spec)
	: m_Spec(spec)
{
}

Mesh::~Mesh() = default;

bool Mesh::LoadFromFile(const std::filesystem::path &path)
{
	Buffer fileData;
	{
		LOG_SCOPE_TIMER("Load file");
		fileData = FileUtil::ReadBinaryFileToBuffer(path);
	}
	if (!fileData)
		return false;

	BufferReader reader = BufferReader(fileData);
	u32          magic  = reader.Read<u32>();
	if (magic != 0x53574150) // "PAWS"
	{
		SIBOX_ERROR("Mesh file \"{}\" is not a valid PAWS file", path.string());
		return false;
	}
	u32 submeshCount = reader.Read<u32>();
	
	m_Meshes.clear();
	m_Meshes.reserve(submeshCount);

	{
		LOG_SCOPE_TIMER("Read submeshes");
		for (u32 i = 0; i < submeshCount; i++)
		{
			m_Meshes.emplace_back();
			if (!m_Meshes[i].LoadFromBuffer(reader))
				return false;
		}	
	}

	return true;
}

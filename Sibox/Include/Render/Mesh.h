#pragma once
#include "VertexArray.h"

struct MeshVertex
{
	Vector3F Position;
	Vector3F Normal;
	Vector2F TexCoord;

	static BufferLayout GetLayout()
	{
		return BufferLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Normal", ShaderDataType::Float3},
			{"a_TexCoord", ShaderDataType::Float2},
		});
	}
};

class SubMesh
{
public:
	SubMesh();
	~SubMesh();

	SubMesh(const SubMesh &other)                = delete;
	SubMesh(SubMesh &&other) noexcept;
	SubMesh& operator=(const SubMesh &other)     = delete;
	SubMesh& operator=(SubMesh &&other) noexcept = delete;

	// Operations
	bool LoadFromBuffer(BufferReader &buffer);

	// Getters
	NODISCARD FORCEINLINE u32 GetNumVerts() const { return m_NumVerts; }
	NODISCARD FORCEINLINE u32 GetNumIndices() const { return m_NumIndices; }
	NODISCARD FORCEINLINE u32 GetMaterialIndex() const { return m_MaterialIndex; }

	// Setters
	void SetMaterialIndex(u32 index) { m_MaterialIndex = index; }

protected:
	u32         m_NumVerts = 0, m_NumIndices = 0, m_MaterialIndex = 0;
	MeshVertex *m_Vertices = nullptr;
	u16 *       m_Indices  = nullptr;

	VertexArray m_VertexArray;
};

struct MeshSpecification
{
	// Empty struct for now - but it'll contain mesh data in the future.
};

class Mesh
{
public:
	explicit Mesh(const MeshSpecification &spec);
	~Mesh();

	Mesh(const Mesh &other)                = delete;
	Mesh(Mesh &&other) noexcept            = delete;
	Mesh& operator=(const Mesh &other)     = delete;
	Mesh& operator=(Mesh &&other) noexcept = delete;

	bool LoadFromFile(const std::filesystem::path &path);

	NODISCARD FORCEINLINE const std::vector<SubMesh>& GetSubMeshes() { return m_Meshes; } 

protected:
	std::vector<SubMesh> m_Meshes;
	MeshSpecification    m_Spec;
};

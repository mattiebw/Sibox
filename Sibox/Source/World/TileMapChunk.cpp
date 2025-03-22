#include "siboxpch.h"
#include "World/TileMapChunk.h"

#include "Core/Application.h"
#include "Core/Random.h"
#include "Render/Renderer.h"
#include "Render/SpriteSheet.h"
#include "Render/VertexArray.h"
#include "World/TileMap.h"

TileMapChunk::TileMapChunk()
	: m_Position(0), m_Size(0), m_TileMap(nullptr)
{
}

TileMapChunk::TileMapChunk(TileMap *tileMap, Vector2I position, Vector2I size)
	: m_Position(position), m_Size(size), m_TileMap(tileMap)
{
	m_Tiles          = new u32[m_Size.X * m_Size.Y];
	memset(m_Tiles, 0, sizeof(u32) * m_Size.X * m_Size.Y);

	if (!Application::Get()->HasFrontend())
		return;
	
	m_TileShaderData = new TileShaderData[m_Size.X * m_Size.Y];
	memset(m_TileShaderData, 0, sizeof(TileShaderData) * m_Size.X * m_Size.Y);

	// Our tile data buffer is just a contiguous buffer of vec2s, representing the top left texture coordinates of the tiles.
	m_TileDataBuffer = CreateRef<VertexBuffer>(static_cast<u32>(sizeof(TileShaderData) * m_Size.X * m_Size.Y),
	                                           BufferUsageType::DynamicDraw);
	m_TileDataBuffer->SetLayout(BufferLayout({
		{"a_TexCoordTopLeft", ShaderDataType::Float2, 0, 1},
		{"a_Rot", ShaderDataType::Float, 0, 1},
	}));

	m_VertexArray = CreateRef<VertexArray>();
	m_VertexArray->AddVertexBuffer(Application::GetRenderer()->GetTileQuadVertexBuffer());
	m_VertexArray->AddVertexBuffer(m_TileDataBuffer);
	m_VertexArray->SetIndexBuffer(Application::GetRenderer()->GetTileQuadIndexBuffer());
}

TileMapChunk::~TileMapChunk()
{
	delete[] m_Tiles;
	delete[] m_TileShaderData;
}

TileData& TileMapChunk::GetTileDataForTile(int x, int y) const
{
	return m_TileMap->GetTileSet()->GetTile(GetTile(x, y));
}

void TileMapChunk::SetTile(int x, int y, u32 tile)
{
	int index                   = y * m_Size.X + x;
	m_Tiles[index]              = tile;
	if (Application::Get()->HasFrontend())
		m_TileShaderData[index].Rot = static_cast<f32>(Random::Int(0, 4));
	m_DataDirty                 = true;
}

void TileMapChunk::UpdateTileData()
{
	if (!Application::Get()->HasFrontend())
		return;
	
	if (!m_DataDirty)
		return;

	for (int i = 0; i < m_Size.X * m_Size.Y; i++)
	{
		if (m_Tiles[i] == 0)
		{
			m_TileShaderData[i].TopLeftTexCoord = Vector2F(-1.0f);
			continue;
		}
		
		SpriteSheetSprite tile              = m_TileMap->GetTileSet()->GetSpriteForTile(m_Tiles[i]);
		m_TileShaderData[i].TopLeftTexCoord = tile.TexCoordsMin;
	}

	m_TileDataBuffer->SetData(m_TileShaderData, static_cast<u32>(m_Size.X * m_Size.Y * sizeof(TileShaderData)));
	m_DataDirty = false;
}

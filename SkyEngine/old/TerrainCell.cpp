#include "stdafx.h"
#include "TerrainCell.h"
#include "globals.h"
#include "Utils.h"
#include <algorithm>

namespace sky
{
	TerrainCell::TerrainCell() 
		: m_visible(false), 
		m_DebugMesh(nullptr)
	{
	}

	TerrainCell::~TerrainCell()
	{
		
	}

	bool TerrainCell::init(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indices, size_t nodeIndexX, size_t nodeIndexY, size_t cellHeight, size_t cellWidth, int terrainWidth)
	{
		unsigned int vertexIndex = nodeIndexX * (cellWidth - 1) + nodeIndexY * ((cellHeight - 1) * terrainWidth);

		float maxX = FLT_MIN;
		float minX = FLT_MAX;
		float maxY = FLT_MIN;
		float minY = FLT_MAX;
		float maxZ = FLT_MIN;
		float minZ = FLT_MAX;

		unsigned int * cellVertexIndexes = new unsigned int[cellHeight * cellWidth];

		unsigned int index = 0;
		for (size_t j = 0; j< cellHeight; j++)
		{
			for (size_t i = 0; i< cellWidth; i++)
			{
				cellVertexIndexes[index] = vertexIndex;

				StaticMeshVertex vertex = verticess[vertexIndex];

				float x = vertex.position.x;
				float y = vertex.position.y;
				float z = vertex.position.z;

				if (x > maxX)  maxX = x;
				if (x < minX)  minX = x;
				if (y > maxY)  maxY = y;
				if (y < minY)  minY = y;
				if (z > maxZ)  maxZ = z;
				if (z < minZ)  minZ = z;

				vertexIndex++;
				index++;
			}
			vertexIndex += terrainWidth - cellWidth;
		}

		for (DWORD i = 0; i < cellHeight - 1; i++)
		{
			for (DWORD j = 0; j < cellWidth - 1; j++)
			{
				indices.push_back(cellVertexIndexes[i*cellWidth + j]);
				indices.push_back(cellVertexIndexes[i*cellWidth + j + 1]);
				indices.push_back(cellVertexIndexes[(i + 1)*cellWidth + j]);

				indices.push_back(cellVertexIndexes[(i + 1)*cellWidth + j]);
				indices.push_back(cellVertexIndexes[i*cellWidth + j + 1]);
				indices.push_back(cellVertexIndexes[(i + 1)*cellWidth + j + 1]);
			}
		}

		if (cellVertexIndexes)
		{
			delete [] cellVertexIndexes;
			cellVertexIndexes = nullptr;
		}

		Utils::calculateAABB(m_AABB, maxX, minX, maxY, minY, maxZ, minZ);

		return true;
	}

	void TerrainCell::isVisible(bool visible)
	{
		m_visible = visible;
	}

	bool TerrainCell::isVisible() const
	{
		return m_visible;
	}

	const BoundingBox *TerrainCell::getBoundingBox() const
	{
		return &m_AABB;
	}

	void TerrainCell::setDebugMesh(ColorMesh * mesh)
	{
		m_DebugMesh = mesh;
	}

	ColorMesh *TerrainCell::getDebugMesh() const
	{
		return m_DebugMesh;
	}

	void TerrainCell::updateCB(CBPerObject *cbPerObject) const
	{
		m_DebugMesh->getRenderData()->updateConstantBuffer<EShaderType::ST_Vertex>(0, cbPerObject);
	}
}

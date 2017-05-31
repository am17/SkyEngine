#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
#include <vector>
#include "VertexFormat.h"
#include "StaticMeshRenderData.h"
#include "DirectXCollision.h"

using namespace DirectX;
using namespace std;
using namespace render;

namespace sky
{
	struct SHeightMapPoint
	{
		float y;
		float r;
		float g;
		float b;
	};

	class TerrainCell;

	class TerrainGeometry
	{
	public:
		TerrainGeometry(int terrainHeight, int terrainWidth);
		~TerrainGeometry();
		bool loadHeightMapsFromBitmap(const char* heightMapFile);
		bool loadHeightMapsFromRaw(const char* heightMapFile);
		void createMesh(vector<StaticMeshVertex>& vertices, vector<unsigned int>& indices);
		bool loadColorMap(const char* colorMapFile) const;
		TerrainCell *getCell(int index) const;
	private:
		void initCells(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indicess);
		SHeightMapPoint *m_heightMap;
		int m_terrainHeight, m_terrainWidth;
		TerrainCell *m_terrainCells;
		size_t m_cellCount;
	};

	class TerrainCell
	{
	public:
		TerrainCell();
		~TerrainCell();
		bool init(vector<StaticMeshVertex>& verticess, size_t i, size_t j, size_t cellHeight, size_t cellWidth, int m_terrainWidth);
		StaticMeshRenderData *getRenderData() const;
		const BoundingBox *getBoundingBox() const;
	private:
		StaticMeshRenderData *m_renderData;
		size_t m_vertexCount;
		size_t m_indexCount;
		BoundingBox m_AABB;
	};
}


#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
#include <vector>
#include "TerrainCell.h"
#include "Material.h"

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

	class TerrainMesh
	{
	public:
		TerrainMesh(int terrainHeight, int terrainWidth);
		~TerrainMesh();
		SHeightMapPoint *loadHeightMapsFromRaw(const char* heightMapFile) const;
		bool loadColorMap(const char* colorMapFile, SHeightMapPoint *m_heightMap) const;
		TerrainCell *getCell(int index) const;
		size_t getCellsCount() const;
		SKYENGINEDLL IMaterial *getMaterial(int elementIndex) const;
		void setMaterial(int elementIndex, IMaterial* material);
		StaticMeshRenderData *getRenderData() const;
		void initCells(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indices);
	private:
		StaticMeshRenderData *renderData;
		int m_terrainHeight, m_terrainWidth;
		TerrainCell *m_terrainCells;
		size_t m_cellCount;
		vector<IMaterial*> materials;
	};
}


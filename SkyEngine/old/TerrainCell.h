#pragma once
#include <vector>
#include "VertexFormat.h"
#include "StaticMeshRenderData.h"
#include "DirectXCollision.h"
#include "ColorMesh.h"
#include "ConstantBufferFormats.h"

using namespace DirectX;
using namespace std;
using namespace render;

namespace sky
{
	class TerrainCell
	{
	public:
		TerrainCell();
		~TerrainCell();
		bool init(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indices, size_t i, size_t j, size_t cellHeight, size_t cellWidth, int m_terrainWidth);
		
		void isVisible(bool visible);
		bool isVisible() const;
		const BoundingBox *getBoundingBox() const;
		void setDebugMesh(ColorMesh * mesh);
		ColorMesh *getDebugMesh() const;
		void updateCB(CBPerObject *cbPerObject) const;
	private:
		BoundingBox m_AABB;
		bool m_visible;
		ColorMesh *m_DebugMesh;
	};
}


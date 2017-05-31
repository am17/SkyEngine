#pragma once
#include "VertexFormat.h"
#include <vector>
#include "Material.h"
#include "common.h"
#include "StaticMeshRenderData.h"

using namespace render;
using namespace std;

namespace sky
{
	class StaticMesh
	{
	public:
		StaticMesh();
		StaticMesh(StaticMeshRenderData* pRenderData);
		~StaticMesh();
		StaticMeshRenderData* getRenderData() const;
		size_t getMaterialsCount() const;
		void setMaterialsCount(size_t materialsCount);
		SKYENGINEDLL IMaterial *getMaterial(int elementIndex) const;
		void setMaterial(int elementIndex, IMaterial* material);
	private:
		StaticMeshRenderData* renderData;
		vector<IMaterial*> materials;
	};
}


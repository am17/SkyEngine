#include "stdafx.h"
#include "StaticMesh.h"
#include "GeometricPrimitive.h"
#include "Material.h"

using namespace DirectX;

namespace sky
{
	StaticMesh::StaticMesh(): 
		renderData(nullptr)
	{
		renderData = new StaticMeshRenderData();
	}

	StaticMesh::StaticMesh(StaticMeshRenderData* pRenderData)
	{
		renderData = pRenderData;
	}

	StaticMesh::~StaticMesh()
	{
		for (size_t i = 0; i < materials.size(); i++)
		{
			if (materials[i])
			{
				delete materials[i];
				materials[i] = nullptr;
			}
		}

		if (renderData)
		{
			delete renderData;
			renderData = nullptr;
		}
	}

	StaticMeshRenderData* StaticMesh::getRenderData() const
	{
		return renderData;
	}

	size_t StaticMesh::getMaterialsCount() const
	{
		return materials.size();
	}

	void StaticMesh::setMaterialsCount(size_t materialsCount)
	{
		materials.reserve(materialsCount);
		materials.resize(materialsCount);
	}

	IMaterial* StaticMesh::getMaterial(int elementIndex) const
	{
		assert(materials.size() > elementIndex);

		return materials[elementIndex];
	}

	void StaticMesh::setMaterial(int elementIndex, IMaterial* material)
	{
		assert(elementIndex >= 0 && material != nullptr);

		PSStage *psStage = renderData->getPipelineState()->getPSStageData();
		VSStage *vsStage = renderData->getPipelineState()->getVSStageData();

		MaterialObserver *observer = new MaterialObserver(*psStage, *vsStage);

		material->addObserver(observer);

		if (materials.size() <= elementIndex)
		{
			materials.push_back(material);
		}
		else
		{
			if (materials[elementIndex])
			{
				delete materials[elementIndex];
				materials[elementIndex] = nullptr;
			}

			materials[elementIndex] = material;
		}
	}
}

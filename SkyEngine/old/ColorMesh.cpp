#include "stdafx.h"
#include "ColorMesh.h"

namespace sky
{
	ColorMesh::ColorMesh(): 
		renderData(nullptr)
	{
		renderData = new ColorMeshRenderData();
	}


	ColorMesh::~ColorMesh()
	{
		if (renderData)
		{
			delete renderData;
			renderData = nullptr;
		}
	}

	ColorMeshRenderData* ColorMesh::getRenderData() const
	{
		return renderData;
	}
}

#pragma once
#include "ColorMeshRenderData.h"

namespace sky
{
	class ColorMesh
	{
	public:
		ColorMesh();
		~ColorMesh();
		ColorMeshRenderData* getRenderData() const;
	private:
		ColorMeshRenderData* renderData;
	};
}


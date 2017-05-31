#pragma once
#include "DirectXCollision.h"

using namespace DirectX;

namespace sky
{
	class Utils
	{
	public:
		static void calculateAABB(BoundingBox &AABB, float maxX, float minX, float maxY, float minY, float maxZ, float minZ)
		{
			XMFLOAT3 bbCenter = XMFLOAT3(((maxX - minX) / 2) + minX, ((maxY - minY) / 2) + minY, ((maxZ - minZ) / 2) + minZ);
			XMFLOAT3 extents = XMFLOAT3((maxX - minX) / 2, (maxY - minY) / 2, (maxZ - minZ) / 2);

			AABB = BoundingBox(bbCenter, extents);
		}
	};
}
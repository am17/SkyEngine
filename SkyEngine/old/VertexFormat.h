#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace PackedVector;

namespace render
{
	struct InstanceType
	{
		XMFLOAT3 position;
	};

	struct StaticMeshVertex
	{
		StaticMeshVertex(){}
		StaticMeshVertex(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT2 UVs)
			:position(position), normal(normal), UVs(UVs){}

		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT2 UVs;
		XMFLOAT4 color;
	};

	struct StaticMeshVertexPack
	{
		StaticMeshVertexPack(){}
		StaticMeshVertexPack(XMFLOAT3 position, XMBYTEN4 normal, XMFLOAT2 UVs)
			:position(position), normal(normal), UVs(UVs){}

		XMFLOAT3 position;
		XMBYTEN4 normal;
		XMBYTEN4 tangent;
		XMFLOAT2 UVs;
		XMUBYTEN4 color;
	};

	struct ColorVertex
	{
		ColorVertex(){}
		ColorVertex(XMFLOAT3 position, XMFLOAT4 color)
			:position(position), color(color){}
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
}
#pragma once
#include "Mesh.h"
#include "Systems\RenderSystem\IDeviceImpl.h"
#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class MeshFactory
{
public:
	MeshFactory(IDeviceImpl* device);
	~MeshFactory();
	Mesh* CreateTriangle();
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	IDeviceImpl* pDevice;
};
#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "Systems\RenderSystem\Resource.h"
#include <vector>

using namespace std;

class D3D11VertexDeclaration : public VertexDeclaration
{
public:
	D3D11VertexDeclaration(vector<D3D11_INPUT_ELEMENT_DESC>& InElements)
		:VertexElements(InElements)
	{}
	vector<D3D11_INPUT_ELEMENT_DESC> VertexElements;
};

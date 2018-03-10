#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "Systems\RenderSystem\Resource.h"

class D3D11DepthStencilState : public DepthStencilState
{
public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> Resource;
};

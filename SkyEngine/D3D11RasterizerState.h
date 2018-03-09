#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "Resource.h"

class D3D11RasterizerState: public RasterizerState
{
public:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> Resource;
};
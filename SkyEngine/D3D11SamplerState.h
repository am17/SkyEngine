#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "Resource.h"

class D3D11SamplerState : public SamplerState
{
public:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> Resource;
};
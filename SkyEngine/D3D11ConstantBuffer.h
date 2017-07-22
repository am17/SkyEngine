#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "Resource.h"

class D3D11ConstantBuffer : public ConstantBuffer
{
public:
	D3D11ConstantBuffer(unsigned int aSize): ConstantBuffer(aSize) {}
	Microsoft::WRL::ComPtr<ID3D11Buffer> Resource;
};

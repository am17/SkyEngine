#pragma once

#include <wrl/client.h>
#include "d3d11.h"
#include "../IndexBuffer.h"
#include "D3D11Device.h"

class D3D11IndexBuffer : public IndexBuffer
{
public:
	D3D11IndexBuffer(D3D11Device *device, unsigned int stride, unsigned int size, BUFFER_USAGE_FLAGS usage);
	~D3D11IndexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> Resource;
private:
	D3D11Device *pDevice;
};
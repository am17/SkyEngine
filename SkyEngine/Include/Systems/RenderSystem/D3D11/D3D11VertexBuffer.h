#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "../VertexBuffer.h"
#include "D3D11Device.h"

class D3D11VertexBuffer: public VertexBuffer
{
public:
	D3D11VertexBuffer(ID3D11Buffer *resource, unsigned int size, BUFFER_USAGE_FLAGS usage);
	~D3D11VertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> Resource;
};

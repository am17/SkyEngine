#pragma once
#include "../Include/Systems/RenderSystem/D3D11/D3D11VertexBuffer.h"

D3D11VertexBuffer::D3D11VertexBuffer(D3D11Device *device, unsigned int size, BUFFER_USAGE_FLAGS usage):
	VertexBuffer(size, usage), 
	pDevice(device)
{

}
D3D11VertexBuffer::~D3D11VertexBuffer()
{

}

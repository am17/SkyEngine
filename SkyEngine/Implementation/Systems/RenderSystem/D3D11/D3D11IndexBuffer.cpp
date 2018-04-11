#pragma once
#include "../Include/Systems/RenderSystem/D3D11/D3D11IndexBuffer.h"

D3D11IndexBuffer::D3D11IndexBuffer(ID3D11Buffer *resource, unsigned int stride, unsigned int size, BUFFER_USAGE_FLAGS usage):
	IndexBuffer(stride, size, usage),
	Resource(resource)
{

}
D3D11IndexBuffer::~D3D11IndexBuffer()
{

}
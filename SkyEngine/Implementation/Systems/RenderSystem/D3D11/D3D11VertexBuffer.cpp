#pragma once
#include "../Include/Systems/RenderSystem/D3D11/D3D11VertexBuffer.h"

D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Buffer *resource, unsigned int size, BUFFER_USAGE_FLAGS usage):
	VertexBuffer(size, usage), 
	Resource(resource)
{

}
D3D11VertexBuffer::~D3D11VertexBuffer()
{

}

#pragma once
#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11VertexBufferImpl : public IVertexBufferImpl
{
public:
	D3D11VertexBufferImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pVertexBuffer(nullptr)
	{

	}
	~D3D11VertexBufferImpl() 
	{
		if (_pVertexBuffer)
		{
			_pVertexBuffer->Release();
			_pVertexBuffer = nullptr;
		}
	}
	bool create(const void *pData, unsigned int elementsCount, unsigned int stride, unsigned int offset) override
	{
		_stride = stride;
		_offset = offset;

		unsigned int bufferSize = stride * elementsCount;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = bufferSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = pData;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result = _pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &_pVertexBuffer);
		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("VertexBuffer create error\n");
			return false;
		}
	}
	void bind(unsigned int startSlot) override
	{
		_pDeviceContext->IASetVertexBuffers(startSlot, 1, &_pVertexBuffer, &_stride, &_offset);
	}
private:
	ID3D11Buffer *_pVertexBuffer;
	unsigned int _stride;
	unsigned int _offset;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
};


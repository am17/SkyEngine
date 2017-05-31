#pragma once
#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11IndexBufferImpl : public IIndexBufferImpl
{
public:
	D3D11IndexBufferImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pIndexBuffer(nullptr),
		_elementsCount(0)
	{}
	~D3D11IndexBufferImpl() 
	{
		if (_pIndexBuffer)
		{
			_pIndexBuffer->Release();
			_pIndexBuffer = nullptr;
		}
	}
	bool create(const void *pData, unsigned int elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format) override
	{
		_elementsCount = elementsCount;

		unsigned int bufferSize = stride * _elementsCount;

		switch (format)
		{
		case INDEX_BUFFER_FORMAT::IB_FORMAT_16:
			_format = DXGI_FORMAT_R16_UINT;
			break;
		case INDEX_BUFFER_FORMAT::IB_FORMAT_32:
			_format = DXGI_FORMAT_R32_UINT;
			break;
		default:
			break;
		}

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = bufferSize;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = pData;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		HRESULT result = _pDevice->CreateBuffer(&indexBufferDesc, &indexData, &_pIndexBuffer);
		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("IndexBuffer create error\n");
			return false;
		}
	}
	void bind(unsigned int offset) override
	{
		_pDeviceContext->IASetIndexBuffer(_pIndexBuffer, _format, offset);
	}
	unsigned int getIndexCount() const override
	{
		return _elementsCount;
	}
private:
	ID3D11Buffer *_pIndexBuffer;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	unsigned int _elementsCount;
	DXGI_FORMAT _format;
};

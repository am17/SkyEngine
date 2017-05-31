#pragma once
#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11ConstantBufferImpl : public IContantBufferImpl
{
public:
	D3D11ConstantBufferImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pConstantBuffer(nullptr) {}
	~D3D11ConstantBufferImpl() 
	{
		if (_pConstantBuffer)
		{
			_pConstantBuffer->Release();
			_pConstantBuffer = nullptr;
		}
	}
	bool create(unsigned int bufferSize, EConstantBufferType type)  override
	{
		_type = type;

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
		constantBufferDesc.ByteWidth = bufferSize;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		switch (_type)
		{
		case EConstantBufferType::CBT_Forever:
			constantBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			constantBufferDesc.CPUAccessFlags = 0;
			break;
		//case EConstantBufferType::CBT_Transient:
		case EConstantBufferType::CBT_Temporary:
		case EConstantBufferType::CBT_Constant:
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case EConstantBufferType::CBT_LongLived:
		default:
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.CPUAccessFlags = 0;
			break;
		}

		HRESULT result = _pDevice->CreateBuffer(&constantBufferDesc, nullptr, &_pConstantBuffer);
		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("ConstantBuffer create error\n");
			return false;
		}
	};
	void bind(sky::EShaderType shaderType, unsigned int startSlot)  override
	{
		switch (shaderType)
		{
		case sky::EShaderType::STVertex:
			_pDeviceContext->VSSetConstantBuffers(startSlot, 1, &_pConstantBuffer);
			break;
		case sky::EShaderType::STHull:
			_pDeviceContext->HSSetConstantBuffers(startSlot, 1, &_pConstantBuffer);
			break;
		case sky::EShaderType::STDomain:
			_pDeviceContext->DSSetConstantBuffers(startSlot, 1, &_pConstantBuffer);
			break;
		case sky::EShaderType::STPixel:
			_pDeviceContext->PSSetConstantBuffers(startSlot, 1, &_pConstantBuffer);
			break;
		case sky::EShaderType::STGeometry:
			_pDeviceContext->GSSetConstantBuffers(startSlot, 1, &_pConstantBuffer);
			break;
		case sky::EShaderType::STCompute:
			_pDeviceContext->CSSetConstantBuffers(startSlot, 1, &_pConstantBuffer);
			break;
		default:
			break;
		}
	};
	void update(void *aSrcData) override
	{
		switch (_type)
		{
		case EConstantBufferType::CBT_Forever:
			{
				//никогда не обновляется
			}
			break;
		//case EConstantBufferType::CBT_Transient:
		case EConstantBufferType::CBT_Temporary:
			{
				D3D11_MAPPED_SUBRESOURCE mapped_res;
				_pDeviceContext->Map(_pConstantBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_res);
				assert(mapped_res.pData);
				mapped_res.pData = aSrcData;
				_pDeviceContext->Unmap(_pConstantBuffer, 0);
			}
			break;
		case EConstantBufferType::CBT_Constant:
			{
				D3D11_MAPPED_SUBRESOURCE mapped_res;
				_pDeviceContext->Map(_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
				assert(mapped_res.pData);
				mapped_res.pData = aSrcData;
				_pDeviceContext->Unmap(_pConstantBuffer, 0);
			}
			break;
		case EConstantBufferType::CBT_LongLived:
		default:
			_pDeviceContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, aSrcData, 0, 0);
			break;
		}
	}
private:
	ID3D11Buffer *_pConstantBuffer;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	EConstantBufferType _type;
};
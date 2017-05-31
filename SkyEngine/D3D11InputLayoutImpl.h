#pragma once

#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11InputLayoutImpl : public IInputLayoutImpl
{
public:
	D3D11InputLayoutImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pInputLayout(nullptr),
		_pLayoutDesc(nullptr)
	{}
	~D3D11InputLayoutImpl()
	{
		if (_pLayoutDesc)
		{
			delete[] _pLayoutDesc;
			_pLayoutDesc = nullptr;
		}

		if (_pInputLayout)
		{
			_pInputLayout->Release();
			_pInputLayout = nullptr;
		}
	}
	bool create(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength)  override
	{
		assert(layoutDescLength <= 32);

		_pLayoutDesc = layoutDesc;

		D3D11_INPUT_ELEMENT_DESC elementDescs[32];

		for (size_t i = 0; i < layoutDescLength; i++)
		{
			elementDescs[i].SemanticName = layoutDesc[i].SemanticName;
			elementDescs[i].SemanticIndex = layoutDesc[i].SemanticIndex;
			elementDescs[i].InputSlot = layoutDesc[i].InputSlot;
			elementDescs[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
			elementDescs[i].InputSlotClass = layoutDesc[i].ForInstance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
			elementDescs[i].InstanceDataStepRate = layoutDesc[i].ForInstance ? 1 : 0;

			switch (layoutDesc[i].Format)
			{
			case ELEMENT_FORMAT::FLOAT2:
				elementDescs[i].Format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case ELEMENT_FORMAT::FLOAT3:
				elementDescs[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case ELEMENT_FORMAT::FLOAT4:
				elementDescs[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			default:
				break;
			}
		}

		HRESULT result = _pDevice->CreateInputLayout(elementDescs, layoutDescLength, pByteCode, ByteCodeLength, &_pInputLayout);

		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("InputLayout create error\n");
			return false;
		}
	};
	void bind()  override
	{
		_pDeviceContext->IASetInputLayout(_pInputLayout);
	};
private:
	ID3D11InputLayout* _pInputLayout;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	VertexLayoutDesc *_pLayoutDesc;
};
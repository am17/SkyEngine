#pragma once
#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11VertexShaderImpl : public IShaderImpl
{
public:
	D3D11VertexShaderImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pVertexShader(nullptr),
		_pByteCode(nullptr),
		_codeLength(0)
	{}
	~D3D11VertexShaderImpl()
	{
		if (_pVertexShader)
		{
			_pVertexShader->Release();
			_pVertexShader = nullptr;
		}
	}
	bool create(const void* pByteCode, size_t ByteCodeLength)  override
	{
		HRESULT result = _pDevice->CreateVertexShader(pByteCode, ByteCodeLength, nullptr, &_pVertexShader);
		if (SUCCEEDED(result))
		{
			_pByteCode = pByteCode;
			_codeLength = ByteCodeLength;

			return true;
		}
		else
		{
			printf("VertexShader create error\n");
			return false;
		}
	};
	void bind()  override
	{
		_pDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	};
	const void* getByteCode() const
	{
		return _pByteCode;
	};
	size_t getCodeLength() const
	{
		return _codeLength;
	};
private:
	ID3D11VertexShader* _pVertexShader;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	const void* _pByteCode;
	size_t _codeLength;
};
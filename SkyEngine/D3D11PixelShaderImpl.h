#pragma once
#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11PixelShaderImpl : public IShaderImpl
{
public:
	D3D11PixelShaderImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pPixelShader(nullptr), 
		_pByteCode(nullptr),
		_codeLength(0)
	{}
	~D3D11PixelShaderImpl()
	{
		if (_pPixelShader)
		{
			_pPixelShader->Release();
			_pPixelShader = nullptr;
		}
	}
	bool create(const void* pByteCode, size_t ByteCodeLength)  override
	{
		HRESULT result = _pDevice->CreatePixelShader(pByteCode, ByteCodeLength, nullptr, &_pPixelShader);
		if (SUCCEEDED(result))
		{
			_pByteCode = pByteCode;
			_codeLength = ByteCodeLength;

			return true;
		}
		else
		{
			printf("PixelShader create error\n");
			return false;
		}
	};
	void bind()  override
	{
		_pDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);
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
	ID3D11PixelShader* _pPixelShader;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	const void* _pByteCode;
	size_t _codeLength;
};
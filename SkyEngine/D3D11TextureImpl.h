#pragma once
#include "DeviceResourceImpl.h"
#include <d3d11.h>
#include <stdio.h>
#include "DDSTextureLoader.h"
#include "ScreenGrab.h"

using namespace DirectX;

class D3D11TextureImpl : public ITextureImpl
{
public:
	D3D11TextureImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pTextureView(nullptr),
		_pRenderTarget(nullptr)
	{

	}
	D3D11TextureImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, ID3D11ShaderResourceView* aTexture)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		_pTextureView(aTexture)
	{

	}
	~D3D11TextureImpl()
	{
		if (_pRenderTarget)
		{
			_pRenderTarget->Release();
			_pRenderTarget = nullptr;
		}

		if (_pTextureView)
		{
			_pTextureView->Release();
			_pTextureView = nullptr;
		}
	}
	bool createFromFile(wchar_t* filename) override
	{
		HRESULT result = CreateDDSTextureFromFile(_pDevice, filename, nullptr, &_pTextureView, 0, nullptr);

		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("CreateTextureFromFile error\n");
			return false;
		}
	}
	bool createTexture1D(unsigned int width, const void* data) override
	{
		ID3D11Texture1D* pTexture = nullptr;

		D3D11_TEXTURE1D_DESC tex_desc;
		tex_desc.Width = width;
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.Usage = D3D11_USAGE_IMMUTABLE;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = data;
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		HRESULT result = 0;
		if (data != nullptr)
		{
			result = _pDevice->CreateTexture1D(&tex_desc, &init_data, &pTexture);
		}
		else
		{
			result = _pDevice->CreateTexture1D(&tex_desc, nullptr, &pTexture);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = tex_desc.Format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		srv_desc.Texture1D.MipLevels = 1;
		srv_desc.Texture1D.MostDetailedMip = 0;

		result = _pDevice->CreateShaderResourceView(pTexture, &srv_desc, &_pTextureView);

		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("createTexture1D error\n");
			return false;
		}
	}
	bool createTexture2D(unsigned int width, unsigned int height, const void* data)
	{
		ID3D11Texture2D* pTexture = nullptr;

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT result = 0;
		if (data != nullptr)
		{
			D3D11_SUBRESOURCE_DATA init_data;
			init_data.pSysMem = data;
			init_data.SysMemPitch = 0;
			init_data.SysMemSlicePitch = 0;

			result = _pDevice->CreateTexture2D(&textureDesc, &init_data, &pTexture);
		}
		else
		{
			result = _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = textureDesc.Format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Texture2D.MostDetailedMip = 0;

		result = _pDevice->CreateShaderResourceView(pTexture, &srv_desc, &_pTextureView);

		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			printf("createTexture2D error\n");
			return false;
		}
	}
	void bind(sky::EShaderType shaderType, unsigned int startSlot) override
	{
		//printf("%d %x\n", startSlot, _pTextureView);

		switch (shaderType)
		{
		case sky::EShaderType::STVertex:
			_pDeviceContext->VSSetShaderResources(startSlot, 1, &_pTextureView); break;
			break;
		case sky::EShaderType::STHull:
			_pDeviceContext->HSSetShaderResources(startSlot, 1, &_pTextureView); break;
			break;
		case sky::EShaderType::STDomain:
			_pDeviceContext->DSSetShaderResources(startSlot, 1, &_pTextureView); break;
			break;
		case sky::EShaderType::STPixel:
			_pDeviceContext->PSSetShaderResources(startSlot, 1, &_pTextureView); break;
			break;
		case sky::EShaderType::STGeometry:
			_pDeviceContext->GSSetShaderResources(startSlot, 1, &_pTextureView); break;
			break;
		case sky::EShaderType::STCompute:
			_pDeviceContext->CSSetShaderResources(startSlot, 1, &_pTextureView); break;
			break;
		default:
			break;
		}
	}
	void unbind(sky::EShaderType shaderType, unsigned int startSlot) override
	{
		switch (shaderType)
		{
		case sky::EShaderType::STVertex:
			_pDeviceContext->VSSetShaderResources(startSlot, 1, nullptr); break;
			break;
		case sky::EShaderType::STHull:
			_pDeviceContext->HSSetShaderResources(startSlot, 1, nullptr); break;
			break;
		case sky::EShaderType::STDomain:
			_pDeviceContext->DSSetShaderResources(startSlot, 1, nullptr); break;
			break;
		case sky::EShaderType::STPixel:
			_pDeviceContext->PSSetShaderResources(startSlot, 1, nullptr); break;
			break;
		case sky::EShaderType::STGeometry:
			_pDeviceContext->GSSetShaderResources(startSlot, 1, nullptr); break;
			break;
		case sky::EShaderType::STCompute:
			_pDeviceContext->CSSetShaderResources(startSlot, 1, nullptr); break;
			break;
		default:
			break;
		}
	}
	void saveToFile(const wchar_t* fileName)
	{
		ID3D11Resource* pSource = nullptr;
		_pTextureView->GetResource(&pSource);
		SaveDDSTextureToFile(_pDeviceContext, pSource, fileName);
	}
private:
	ID3D11ShaderResourceView* _pTextureView;
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	ID3D11RenderTargetView* _pRenderTarget;
};
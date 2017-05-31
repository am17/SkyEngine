#pragma once
#include "DeviceResourceImpl.h"
#include "D3D11TextureImpl.h"
#include <d3d11.h>
#include <stdio.h>

class D3D11RenderTargetImpl : public IRenderTargetImpl
{
public:
	D3D11RenderTargetImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		format(DXGI_FORMAT_R32G32B32A32_FLOAT),
		_renderTargetView(nullptr),
		pTexture(nullptr),
		_textureImpl(nullptr),
		created(false)
	{}
	D3D11RenderTargetImpl(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, ID3D11Texture2D* aTexture)
		:_pDevice(aDevice),
		_pDeviceContext(aDeviceContext),
		format(DXGI_FORMAT_R32G32B32A32_FLOAT),
		_renderTargetView(nullptr),
		pTexture(aTexture),
		_textureImpl(nullptr),
		created(false)
	{
		HRESULT result = _pDevice->CreateRenderTargetView(pTexture, nullptr, &_renderTargetView);

		assert(SUCCEEDED(result));

		created = true;
	}
	~D3D11RenderTargetImpl()
	{
		if (_renderTargetView)
		{
			_renderTargetView->Release();
			_renderTargetView = nullptr;
		}

		if (pTexture)
		{
			pTexture->Release();
			pTexture = nullptr;
		}
	}
	bool create(unsigned int width, unsigned int height) override
	{
		if (!created)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = format;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			HRESULT result = _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);

			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			renderTargetViewDesc.Format = format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetViewDesc.Texture2D.MipSlice = 0;

			result = _pDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &_renderTargetView);

			assert(createResource());

			if (SUCCEEDED(result))
			{
				return true;
			}
			else
			{
				printf("createrenderTarget error\n");
				return false;
			}
		}

		return true;
	}
	bool createResource()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* _pTexture = nullptr;

		HRESULT result = _pDevice->CreateShaderResourceView(pTexture, &srv_desc, &_pTexture);

		if (SUCCEEDED(result))
		{
			_textureImpl = new D3D11TextureImp(_pDevice, _pDeviceContext, _pTexture);

			return true;
		}
		else
		{
			printf("createrenderTargetResource error\n");
			return false;
		}
	}
	ITextureImpl *getTextureImpl() const override
	{
		return _textureImpl;
	}
	void clear() override
	{
		float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

		_pDeviceContext->ClearRenderTargetView(_renderTargetView, color);
	}
	ID3D11RenderTargetView* getRenderTargetView() const { return _renderTargetView; }
private:
	ID3D11Device* _pDevice;
	ID3D11DeviceContext* _pDeviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11Texture2D* pTexture;
	DXGI_FORMAT format;
	ITextureImpl *_textureImpl;
	bool created;
};
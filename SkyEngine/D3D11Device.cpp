#include "stdafx.h"
#include "D3D11Device.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

D3D11Device::D3D11Device()
{
}


D3D11Device::~D3D11Device()
{
}

bool D3D11Device::Init()
{
	HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)mDXGIFactory.ReleaseAndGetAddressOf());

	if (FAILED(result)) return false;

	if (!mDirect3DDevice)
	{
		D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;

		unsigned int DeviceFlags = 0;

#if _DEBUG
		DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

		const D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		result = D3D11CreateDevice(nullptr, DriverType, nullptr, DeviceFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, mDirect3DDevice.ReleaseAndGetAddressOf(), &FeatureLevel, mD3DDeviceIMContext.ReleaseAndGetAddressOf());

		if (FAILED(result)) return false;
	}

	return true;
}

void *D3D11Device::CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality)
{
	assert(width > 0 && height > 0);
	assert(createRTV != createDSV);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> TextureResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> textureRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> textureDSV;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	D3D11_SHADER_RESOURCE_VIEW_DESC textureSRV_desc;
	ZeroMemory(&textureSRV_desc, sizeof(textureSRV_desc));

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));

	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;

	if (createRTV)
	{
		BindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	if (createDSV)
	{
		BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	}

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = createDSV ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = multiSampleCount;
	textureDesc.SampleDesc.Quality = multiSampleQuality;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = BindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	textureSRV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureSRV_desc.ViewDimension = multiSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	textureSRV_desc.Texture2D.MipLevels = textureDesc.MipLevels;
	textureSRV_desc.Texture2D.MostDetailedMip = 0;

	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = multiSampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = 0;
	DSVDesc.Texture2D.MipSlice = 0;

	HRESULT result = mDirect3DDevice->CreateTexture2D(&textureDesc, 0, TextureResource.ReleaseAndGetAddressOf());
	result = mDirect3DDevice->CreateShaderResourceView(TextureResource.Get(), &textureSRV_desc, textureSRV.ReleaseAndGetAddressOf());
	
	if (createDSV)
	{
		result = mDirect3DDevice->CreateDepthStencilView(TextureResource.Get(), &DSVDesc, textureDSV.ReleaseAndGetAddressOf());
	}

	if (createRTV)
	{
		result = mDirect3DDevice->CreateRenderTargetView(TextureResource.Get(), nullptr, textureRTV.ReleaseAndGetAddressOf());
	}

	if (FAILED(result)) return nullptr;

	return TextureResource.Get();
}
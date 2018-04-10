#pragma once
#include "D3D11Resources.h"
#include "D3D11Device.h"
#include "utility.h"

class D3D11Viewport : public ViewportImpl
{
public:
	D3D11Viewport(D3D11Device *pDevice, HWND InWindowHandle, unsigned int InSizeX, unsigned int InSizeY, bool bInIsFullscreen, PIXEL_FORMAT InPreferredPixelFormat)
		: mDevice(pDevice),
		WindowHandle(InWindowHandle),
		SizeX(InSizeX),
		SizeY(InSizeY),
		bIsFullscreen(bInIsFullscreen),
		PixelFormat(InPreferredPixelFormat)
	{
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		SwapChainDesc.BufferDesc = SetupDXGI_MODE_DESC();
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = WindowHandle;
		SwapChainDesc.Windowed = !bIsFullscreen;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ComPtr<IDXGIDevice> DXGIDevice;
		ThrowIfFailed(mDevice->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.ReleaseAndGetAddressOf()));

		IDXGIFactory1 *factory = mDevice->GetFactory();
		ThrowIfFailed(factory->CreateSwapChain(DXGIDevice.Get(), &SwapChainDesc, SwapChain.ReleaseAndGetAddressOf()));

		D3D11Texture2D* backbufferSurface = GetSwapChainSurface(PixelFormat, SwapChain.Get());

		BackBuffer = shared_ptr<D3D11Texture2D>(backbufferSurface);

		::PostMessage(WindowHandle, WM_PAINT, 0, 0);
	}
	~D3D11Viewport() {}

	void Resize(unsigned int InSizeX, unsigned int InSizeY, bool bInIsFullscreen, PIXEL_FORMAT PreferredPixelFormat)
	{

	}
	void ConditionalResetSwapChain(bool bIgnoreFocus)
	{

	}
	bool Present(bool bLockToVsync) override
	{
		bool bNativelyPresented = true;

		const bool bSyncWithDWM = bLockToVsync && !bIsFullscreen;

		if (bSyncWithDWM)
		{
			PresentWithVsyncDWM();
		}
		else
		{
			bNativelyPresented = PresentChecked(bLockToVsync ? 1 : 0);
		}

		return bNativelyPresented;
	}

	Texture2DImpl* GetBackBuffer() const { return BackBuffer.get(); }
	IDXGISwapChain* GetSwapChain() const { return SwapChain.Get(); }
	virtual void* GetNativeSwapChain() const override { return GetSwapChain(); }
	virtual void* GetNativeBackBufferTexture() const override { return BackBuffer->GetResource(); }
	virtual void* GetNativeBackBufferRT() const override { return BackBuffer->getRenderTargetView(0, 0); }
	virtual void* GetNativeWindow(void** AddParam = nullptr) const override { return (void*)WindowHandle; }
private:
	DXGI_MODE_DESC SetupDXGI_MODE_DESC() const
	{
		DXGI_MODE_DESC Ret;

		Ret.Width = SizeX;
		Ret.Height = SizeY;
		Ret.RefreshRate.Numerator = 0;
		Ret.RefreshRate.Denominator = 0;
		Ret.Format = GetRenderTargetFormat(PixelFormat);
		Ret.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		Ret.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		return Ret;
	}

	static DXGI_FORMAT GetRenderTargetFormat(PIXEL_FORMAT PixelFormat)
	{
		/*DXGI_FORMAT	DXFormat = (DXGI_FORMAT)GPixelFormats[PixelFormat].PlatformFormat;
		switch (DXFormat)
		{
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:		return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_BC1_TYPELESS:			return DXGI_FORMAT_BC1_UNORM;
		case DXGI_FORMAT_BC2_TYPELESS:			return DXGI_FORMAT_BC2_UNORM;
		case DXGI_FORMAT_BC3_TYPELESS:			return DXGI_FORMAT_BC3_UNORM;
		case DXGI_FORMAT_R16_TYPELESS:			return DXGI_FORMAT_R16_UNORM;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		default: 								return DXFormat;
		}*/

		return DXGI_FORMAT_B8G8R8A8_UNORM;
	}

	D3D11Texture2D* GetSwapChainSurface(PIXEL_FORMAT PixelFormat, IDXGISwapChain* SwapChain)
	{
		ComPtr<ID3D11Texture2D> BackBufferResource;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)BackBufferResource.ReleaseAndGetAddressOf());

		ComPtr<ID3D11RenderTargetView> BackBufferRenderTargetView;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		RTVDesc.Format = DXGI_FORMAT_UNKNOWN;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(mDevice->GetDevice()->CreateRenderTargetView(BackBufferResource.Get(), &RTVDesc, BackBufferRenderTargetView.ReleaseAndGetAddressOf()));

		D3D11_TEXTURE2D_DESC TextureDesc;
		BackBufferResource->GetDesc(&TextureDesc);

		vector<ComPtr<ID3D11RenderTargetView>> RenderTargetViews;
		RenderTargetViews.push_back(BackBufferRenderTargetView);

		ComPtr<ID3D11ShaderResourceView> BackBufferShaderResourceView;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = 1;

		ThrowIfFailed(mDevice->GetDevice()->CreateShaderResourceView(BackBufferResource.Get(), &SRVDesc, BackBufferShaderResourceView.ReleaseAndGetAddressOf()));

		D3D11Texture2D* NewTexture = new D3D11Texture2D(BackBufferResource.Get(), BackBufferShaderResourceView.Get(), false, 1,RenderTargetViews, nullptr,TextureDesc.Width,TextureDesc.Height,1,1,1,PixelFormat,false,false, false);

		//D3D11TextureAllocated2D(*NewTexture);

		return NewTexture;
	}

	void PresentWithVsyncDWM()
	{
		PresentChecked(0);
	}

	bool PresentChecked(int SyncInterval)
	{
		HRESULT Result = S_OK;
		bool bNeedNativePresent = true;

		if (bNeedNativePresent)
		{
			Result = SwapChain->Present(SyncInterval, 0);
		}

		return bNeedNativePresent;
	}

	shared_ptr<D3D11Texture2D> BackBuffer;
	shared_ptr<D3D11Device> mDevice;
	ComPtr<IDXGISwapChain> SwapChain;
	HWND WindowHandle;
	unsigned int SizeX;
	unsigned int SizeY;
	bool bIsFullscreen;
	PIXEL_FORMAT PixelFormat;
};

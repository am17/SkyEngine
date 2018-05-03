#include "Systems\RenderSystem\D3D11\D3D11ViewPort.h"

D3D11ViewPort::D3D11ViewPort(D3D11Device *device, HWND hWnd, unsigned int backBufferWidth, unsigned int backBufferHeight, bool isFullscreen):
	pDevice(device),
	WindowHandle(hWnd),
	Width(backBufferWidth),
	Height(backBufferHeight),
	IsFullscreen(isFullscreen)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = Width;
	swapChainDesc.BufferDesc.Height = Height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = WindowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !IsFullscreen;

	IDXGIFactory1 *dxgiFactory = pDevice->GetFactory();
	ID3D11Device *m_d3dDevice = pDevice->GetDevice();

	HRESULT hr = dxgiFactory->CreateSwapChain(m_d3dDevice, &swapChainDesc, SwapChain.ReleaseAndGetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	SwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	
	RenderTargetTexture = make_shared<D3D11Texture2D>(pDevice);
	RenderTargetTexture->CreateRenderTarget(backBuffer.Get());
	
	DepthStencilTexture = make_shared<D3D11Texture2D>(pDevice);
	DepthStencilTexture->Create(Width, Height, nullptr, false, true);

	Resource = make_shared<CD3D11_VIEWPORT>(0.f, 0.f, Width, Height);
}

D3D11ViewPort::~D3D11ViewPort()
{
}

void D3D11ViewPort::Clear()
{
	float ClearColor[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

	ID3D11RenderTargetView *rtv = RenderTargetTexture.get()->RenderTargetView.Get();
	ID3D11DepthStencilView *dsv = DepthStencilTexture.get()->DepthStencilView.Get();

	pDevice->GetContext()->ClearRenderTargetView(rtv, ClearColor);
	pDevice->GetContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDevice->GetContext()->OMSetRenderTargets(1, &rtv, dsv);

	pDevice->GetContext()->RSSetViewports(1, Resource.get());
}


void D3D11ViewPort::Present()
{
	SwapChain->Present(0, 0);
}


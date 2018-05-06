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
	
	RenderTargetTexture = device->CreateTexture2D(backBuffer.Get(), true, false, 1, 0);

	DepthStencilTexture = device->CreateTexture2D(backBufferWidth, backBufferHeight, nullptr, false, true, 1, 0);

	Resource = make_shared<CD3D11_VIEWPORT>(0.f, 0.f, Width, Height);
}

D3D11ViewPort::~D3D11ViewPort()
{
}

void D3D11ViewPort::Clear()
{
	pDevice->SetRenderTarget(RenderTargetTexture, DepthStencilTexture, true, true);

	pDevice->SetViewport(this);
}


void D3D11ViewPort::Present()
{
	SwapChain->Present(0, 0);
}


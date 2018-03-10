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
	m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf());

	CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, Width, Height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf());
}

D3D11ViewPort::~D3D11ViewPort()
{
}

void D3D11ViewPort::Clear()
{
	float ClearColor[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

	pDevice->GetContext()->ClearRenderTargetView(m_renderTargetView.Get(), ClearColor);
	pDevice->GetContext()->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDevice->GetContext()->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, Width, Height);
	pDevice->GetContext()->RSSetViewports(1, &viewport);
}


void D3D11ViewPort::Present()
{
	SwapChain->Present(0, 0);
}


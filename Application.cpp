#include "stdafx.h"
#include "Application.h"
#include "D3D11Viewport.h"

Application::Application()
{
}


Application::~Application()
{
}

bool Application::init(HWND window, int width, int height)
{
	mDevice = std::make_shared<D3D11Device>();
	mDevice->init();

	viewPort = std::make_shared<D3D11Viewport>(mDevice.get(), window, width, height, false, PIXEL_FORMAT::PF_A8R8G8B8);

	return true;
}

void Application::tick()
{
	render();
}

void Application::render()
{
	//mDevice->clear()
	float ClearColor[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

	ID3D11DeviceContext* m_d3dContext = mDevice->GetDeviceContext();

	ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)viewPort->GetNativeBackBufferRT();
	Texture2DImpl *bb = viewPort->GetBackBuffer();

	m_d3dContext->ClearRenderTargetView(rtv, ClearColor);
	//m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, &rtv, nullptr);

	mDevice->SetViewport(0.0f, 0.0f, D3D11_MIN_DEPTH, 800, 600, D3D11_MAX_DEPTH);

	viewPort->Present(false);
}

#include "Systems\RenderSystem.h"
#include "Systems\RenderSystem\D3D11\D3D11Device.h"
#include "Systems\RenderSystem\D3D11\D3D11ViewPort.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update(double time)
{
	viewPort->Clear();

	viewPort->Present();
}

void RenderSystem::Start()
{
}

void RenderSystem::Stop()
{
}

void RenderSystem::Init(HWND hWnd, int width, int height)
{
	device = std::make_unique<D3D11Device>();
	device->Init();

	viewPort = std::make_unique<D3D11ViewPort>(static_cast<D3D11Device*>(device.get()), hWnd, width, height, false);
}

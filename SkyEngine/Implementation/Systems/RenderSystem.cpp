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
	printf("%f\n", time);

	viewPort->Clear();

	//TODO: Render here
	//m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

	//m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	//m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	//m_pImmediateContext->Draw(3, 0);

	RenderToBackBuffer();

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
	device = std::make_shared<D3D11Device>();
	device->Init();

	viewPort = std::make_unique<D3D11ViewPort>(static_cast<D3D11Device*>(device.get()), hWnd, width, height, false);

	meshFactory = std::make_unique<MeshFactory>(device.get());

	shaderFactory = std::make_unique<ShaderFactory>(device.get());

	drawingSurface = device->CreateTexture2D(width*main_buffer_size_multiplier, height*main_buffer_size_multiplier, nullptr, true, false, 1, 0);//2?

	drawingSurfaceResolved = device->CreateTexture2D(width*main_buffer_size_multiplier, height*main_buffer_size_multiplier, nullptr, true, false, 1, 0);
}

bool RenderSystem::RegisterEntity(Entity * entity)
{
	return true;
}

IDeviceImpl * RenderSystem::GetDevice() const
{
	return device.get();
}

MeshFactory * RenderSystem::GetMeshFactory() const
{
	return meshFactory.get();
}

ShaderFactory * RenderSystem::GetShaderFactory() const
{
	return shaderFactory.get();
}

void RenderSystem::RenderToBackBuffer()
{
	device->SetViewport(viewPort.get());

	device->SetRasterizerState(COMMON_RASTERIZER_STATES::RS_CULLNONE);
	device->SetDepthStencilState(COMMON_DEPTH_STENCIL_STATES::DSS_DEPTHNONE);
	device->SetBlendState(COMMON_BLEND_STATES::BS_OPAQUE);

	device->SetRenderTarget(viewPort->RenderTargetTexture, viewPort->DepthStencilTexture, false, false);

	device->CopyMsTextureToTexture(drawingSurface, drawingSurfaceResolved);//drawingSurfaceResolved -> g_MainTexture

	//pContext->PSSetShaderResources(12, 1, &g_MainTexture);

	//pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//pContext->VSSetShader(FullScreenQuadVS, nullptr, 0);
	//pContext->HSSetShader(nullptr, nullptr, 0);
	//pContext->DSSetShader(nullptr, nullptr, 0);
	//pContext->PSSetShader(MainToBackBufferPS, nullptr, 0);

	//cbuffer.g_MainBufferSizeMultiplier = main_buffer_size_multiplier;

	//pContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbuffer, 0, 0);

	//device->Draw(4);
}

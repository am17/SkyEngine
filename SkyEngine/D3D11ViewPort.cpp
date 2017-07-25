#include "stdafx.h"
#include "D3D11ViewPort.h"
#include "D3D11Device.h"
#include "ShaderFactory.h"

D3D11ViewPort::D3D11ViewPort(IDeviceImpl *device, unsigned int width, unsigned int height)
	:mDevice(device),
	mOutputWidth(width),
	mOutputHeight(height)
{
	
}


D3D11ViewPort::~D3D11ViewPort()
{
}

bool D3D11ViewPort::Init(HWND ouputWindow, bool IsFullscreen)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencil;

	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	Microsoft::WRL::ComPtr<IDXGIDevice>	DXGIDevice;

	D3D11Device *pDevice = static_cast<D3D11Device*>(mDevice.get());

	ID3D11Device *direct3DDevice = pDevice->GetDevice();
	direct3DDevice->GetImmediateContext(&mDeviceContext);

	HRESULT result = direct3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.ReleaseAndGetAddressOf());

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	DXGI_MODE_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(DXGI_MODE_DESC));

	BufferDesc.Width = mOutputWidth;
	BufferDesc.Height = mOutputHeight;
	BufferDesc.RefreshRate.Numerator = 0;
	BufferDesc.RefreshRate.Denominator = 0;
	BufferDesc.Format = backBufferFormat;
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChainDesc.BufferDesc = BufferDesc;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = ouputWindow;
	SwapChainDesc.Windowed = !IsFullscreen;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIFactory1 *mDXGIFactory = pDevice->GetFactory();

	result = mDXGIFactory->CreateSwapChain(DXGIDevice.Get(), &SwapChainDesc, mSwapChain.ReleaseAndGetAddressOf());

	result = mSwapChain->GetBuffer(0, IID_PPV_ARGS(mBackBuffer.GetAddressOf()));

	result = direct3DDevice->CreateRenderTargetView(mBackBuffer.Get(), nullptr, mRenderTargetView.ReleaseAndGetAddressOf());

	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, mOutputWidth, mOutputHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	result = direct3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencil.GetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	result = direct3DDevice->CreateDepthStencilView(mDepthStencil.Get(), &depthStencilViewDesc, mDepthStencilView.ReleaseAndGetAddressOf());

	ShaderFactory &factory = ShaderFactory::GetInstance(mDevice.get());

	mVertexShader = static_cast<D3D11VertexShader*>(factory.GetShader(L"mainVS", SHADER_TYPE::VERTEX_SHADER, "FullScreenQuadVS"));

	const D3D11_INPUT_ELEMENT_DESC SkyLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = direct3DDevice->CreateInputLayout(SkyLayout, 2, mVertexShader->Code, mVertexShader->ByteCodeLength, trianglestrip_inputlayout.ReleaseAndGetAddressOf());

	mPixelShader = static_cast<D3D11PixelShader*>(factory.GetShader(L"mainPS", SHADER_TYPE::PIXEL_SHADER, "MainToBackBufferPS"));

	SamplerStateDesc SSDesc;
	SSDesc.Filter = TEXTURE_FILTER::LINEAR;
	SSDesc.TextureAddress = SAMPLER_ADDRESS_MODE::WRAP;
	SSDesc.MaxAnisotropy = 16;
	SSDesc.ComparisonFunction = COMPARISON_FUNCTION::NEVER;

	mSamplerLinearWrap = static_cast<D3D11SamplerState*>(pDevice->CreateSamplerState(SSDesc));


	///
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	const D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		FALSE,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_ALL,
	};

	blendDesc.RenderTarget[0] = defaultRenderTargetBlendDesc;
	result = direct3DDevice->CreateBlendState(&blendDesc, mNoBlending.ReleaseAndGetAddressOf());

	m_states = std::make_unique<DirectX::CommonStates>(direct3DDevice);

	return false;
}

void D3D11ViewPort::Clear()
{
	float ClearColor[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), ClearColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(mOutputWidth), static_cast<float>(mOutputHeight));

	mDeviceContext->RSSetViewports(1, &viewport);
}

void D3D11ViewPort::Present(int SyncInterval)
{
	HRESULT hr = mSwapChain->Present(SyncInterval, 0);

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		//OnDeviceLost();
	}
	else
	{
		//DX::ThrowIfFailed(hr);
	}
}

void D3D11ViewPort::RenderToBackBuffer()
{
	mDeviceContext->RSSetState(m_states->CullNone());
	mDeviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);

	const float BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(mNoBlending.Get(), BlendFactor, 0xFFFFFFFF);

	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(mOutputWidth), static_cast<float>(mOutputHeight));

	mDeviceContext->RSSetViewports(1, &viewport);

	mDeviceContext->PSSetSamplers(0, 1, mSamplerLinearWrap->Resource.GetAddressOf());

	mDeviceContext->IASetInputLayout(trianglestrip_inputlayout.Get());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	mDeviceContext->VSSetShader(mVertexShader->Resource.Get(), nullptr, 0);
	mDeviceContext->PSSetShader(mPixelShader->Resource.Get(), nullptr, 0);

	mDeviceContext->Draw(4, 0);
}

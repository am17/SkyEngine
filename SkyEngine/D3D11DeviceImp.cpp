#include "stdafx.h"
#include "D3D11DeviceImp.h"
#include "D3D11VertexBufferImpl.h"
#include "D3D11IndexBufferImpl.h"
#include "D3D11ConstantBufferImpl.h"
#include "D3D11VertexShaderImpl.h"
#include "D3D11PixelShaderImpl.h"
#include "D3D11InputLayoutImpl.h"
#include "D3D11TextureImpl.h"
#include <d3dcompiler.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

D3D11DeviceImpl::D3D11DeviceImpl()
	:m_device(nullptr),
	m_deviceContext(nullptr),
	m_swapChain(nullptr),
	m_depthStencilBuffer(nullptr),
	m_depthStencilState(nullptr),
	m_depthDisabledStencilState(nullptr),
	m_depthStencilStateLessEqual(nullptr),
	m_depthStencil(nullptr),
	m_cullBackRasterState(nullptr),
	m_noCullingrasterState(nullptr),
	m_samplerRepeatMaxAniso(nullptr),
	m_samplerClampLinear(nullptr),
	m_cardName(nullptr),
	m_dedicatedVideoMemory(0),
	m_vsync_enabled(false),
	//m_timer(nullptr),
	m_screenWidth(0),
	m_screenHeight(0),
	m_backBufferRenderTarget(nullptr)
	//_depthMapRenderTarget(nullptr)
	//m_currentState(nullptr)
{
	//memset(currentRenderTargets, 0, sizeof(ID3D11RenderTargetView*) * D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

	m_screenWidth = 0;
}


D3D11DeviceImpl::~D3D11DeviceImpl()
{
	if (m_cardName)
	{
		delete[] m_cardName;
		m_cardName = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	/*if (_depthMapRenderTarget)
	{
		_depthMapRenderTarget->Release();
		_depthMapRenderTarget = nullptr;
	}*/

	if (m_backBufferRenderTarget)
	{
		m_backBufferRenderTarget->Release();
		m_backBufferRenderTarget = nullptr;
	}

	if (m_alphaBlendState)
	{
		m_alphaBlendState->Release();
		m_alphaBlendState = nullptr;
	}

	if (m_samplerClampLinear)
	{
		m_samplerClampLinear->Release();
		m_samplerClampLinear = nullptr;
	}

	if (m_samplerRepeatMaxAniso)
	{
		m_samplerRepeatMaxAniso->Release();
		m_samplerRepeatMaxAniso = nullptr;
	}

	if (m_cullBackRasterState)
	{
		m_cullBackRasterState->Release();
		m_cullBackRasterState = nullptr;
	}

	if (m_depthStencil)
	{
		m_depthStencil->Release();
		m_depthStencil = nullptr;
	}

	if (m_depthStencilStateLessEqual)
	{
		m_depthStencilStateLessEqual->Release();
		m_depthStencilStateLessEqual = nullptr;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}


bool D3D11DeviceImpl::init(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen)
{
	DXGI_FORMAT pixelFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT resourceDataFormat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	fillVideoCardInfo(pixelFormat, m_vsync_enabled);

	if (!createDeviceAndSwapChain(hwnd, pixelFormat, fullscreen))
	{
		return false;
	}

	if (!createDepthStencilBuffer(resourceDataFormat))
	{
		return false;
	}

	if (!createDepthStencilStates())
	{
		return false;
	}

	setDepthState(sky::EDepthState::DS_DEFAULT);

	if (!createBackBufferRenderTarget(resourceDataFormat))
	{
		return false;
	}

	//_currentRenderTarget = ERenderTarget::RT_BACK_BUFFER;

	//setRenderTarget(ERenderTarget::RT_BACK_BUFFER);

	if (!createRasterizerStates())
	{
		return false;
	}

	setCullBackRasterizerState();

	setViewPort();

	createSamplerState();
	createBlendStates();
	//createDepthMapRenderTarget();

	//m_timer = new Timer();

	/*if (!m_timer->start())
	{
		return false;
	}*/

	return true;
}

bool D3D11DeviceImpl::fillVideoCardInfo(DXGI_FORMAT pixelFormat, bool vsyncEnabled)
{
	HRESULT result;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	fillDispayModeRational(adapter, pixelFormat, vsyncEnabled);

	fillVideoCardDescription(adapter);

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	return true;
}
bool D3D11DeviceImpl::createDeviceAndSwapChain(HWND hwnd, DXGI_FORMAT pixelFormat, bool fullscreen)
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_10_0; // D3D_FEATURE_LEVEL_10_0;// D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_screenWidth;
	swapChainDesc.BufferDesc.Height = m_screenHeight;
	swapChainDesc.BufferDesc.Format = pixelFormat;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_dispayModeRational.numerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = m_dispayModeRational.denominator;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//stateCache.init(m_deviceContext);

	return true;
}
bool D3D11DeviceImpl::createDepthStencilBuffer(DXGI_FORMAT resourceDataFormat)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = m_screenWidth;
	depthBufferDesc.Height = m_screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = resourceDataFormat;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRESULT result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;

	result = m_device->CreateShaderResourceView(m_depthStencilBuffer, &srv_desc, &_depthMapTexture);

	if (FAILED(result))
	{
		return false;
	}

	ITextureImpl *textureImpl = new D3D11TextureImpl(m_device, m_deviceContext, _depthMapTexture);

	_depthMap = new Texture(textureImpl);

	return true;
}
bool D3D11DeviceImpl::createDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	result = m_device->CreateDepthStencilState(&dssDesc, &m_depthStencilStateLessEqual);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
bool D3D11DeviceImpl::createBackBufferRenderTarget(DXGI_FORMAT resourceDataFormat)
{
	ID3D11Texture2D* backBufferPtr;

	HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_backBufferRenderTarget);
	if (FAILED(result))
	{
		return false;
	}
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencil);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
bool D3D11DeviceImpl::createRasterizerStates()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = m_device->CreateRasterizerState(&rasterDesc, &m_cullBackRasterState);
	if (FAILED(result))
	{
		return false;
	}

	rasterDesc.CullMode = D3D11_CULL_NONE;
	result = m_device->CreateRasterizerState(&rasterDesc, &m_noCullingrasterState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
void D3D11DeviceImpl::setCullBackRasterizerState()
{
	m_deviceContext->RSSetState(m_cullBackRasterState);
}
void D3D11DeviceImpl::setNoCullBackRasterizerState()
{
	m_deviceContext->RSSetState(m_noCullingrasterState);
}
void D3D11DeviceImpl::setViewPort()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = static_cast<float>(m_screenWidth);
	viewport.Height = static_cast<float>(m_screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//stateCache.setViewport(viewport);
	m_deviceContext->RSSetViewports(1, &viewport);
}
void D3D11DeviceImpl::createSamplerState()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	HRESULT result = m_device->CreateSamplerState(&sampDesc, &m_samplerClampLinear);

	if (FAILED(result))
	{
		//Log::write("createSamplerState ERROR");
	}
	else
	{
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerClampLinear);
	}

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 16;
	result = m_device->CreateSamplerState(&sampDesc, &m_samplerRepeatMaxAniso);
}

void D3D11DeviceImpl::createBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;//Enable blending.

	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;//
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;//

	rtbd.SrcBlendAlpha = D3D11_BLEND_ZERO;//
	rtbd.DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;//
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;//

	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;//

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	m_device->CreateBlendState(&blendDesc, &m_alphaBlendState);
}

bool D3D11DeviceImpl::fillVideoCardDescription(IDXGIAdapter* adapter)
{
	DXGI_ADAPTER_DESC adapterDesc;
	HRESULT result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	m_dedicatedVideoMemory = static_cast<unsigned int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	char Description[128];
	size_t charNum;
	int error = wcstombs_s(&charNum, Description, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	m_cardName = new char[128];

	strcpy_s(m_cardName, 128, Description);

	return true;
}
bool D3D11DeviceImpl::fillDispayModeRational(IDXGIAdapter* adapter, DXGI_FORMAT pixelFormat, bool vsyncEnabled)
{
	m_dispayModeRational.numerator = 0;
	m_dispayModeRational.denominator = 1;

	if (vsyncEnabled)
	{
		IDXGIOutput* adapterOutput;
		unsigned int numModes;

		HRESULT result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		result = adapterOutput->GetDisplayModeList(pixelFormat, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		result = adapterOutput->GetDisplayModeList(pixelFormat, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		for (unsigned int i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == static_cast<unsigned int>(m_screenWidth))
			{
				if (displayModeList[i].Height == static_cast<unsigned int>(m_screenHeight))
				{
					m_dispayModeRational.numerator = displayModeList[i].RefreshRate.Numerator;
					m_dispayModeRational.denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		delete[] displayModeList;

		adapterOutput->Release();
		adapterOutput = nullptr;
	}

	return true;
}

//bool D3D11DeviceImpl::createDepthMapRenderTarget()
//{
//	D3D11_TEXTURE2D_DESC textureDesc;
//	ZeroMemory(&textureDesc, sizeof(textureDesc));
//	textureDesc.Width = m_screenWidth;
//	textureDesc.Height = m_screenHeight;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1;
//	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = 0;
//
//	ID3D11Texture2D* pTexture = nullptr;
//	HRESULT result = m_device->CreateTexture2D(&textureDesc, nullptr, &pTexture);
//
//	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
//	renderTargetViewDesc.Format = textureDesc.Format;
//	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//	renderTargetViewDesc.Texture2D.MipSlice = 0;
//
//	result = m_device->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &_depthMapRenderTarget);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
//	srv_desc.Format = textureDesc.Format;
//	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srv_desc.Texture2D.MipLevels = 1;
//	srv_desc.Texture2D.MostDetailedMip = 0;
//
//	result = m_device->CreateShaderResourceView(pTexture, &srv_desc, &_depthMapTexture);
//
//	return true;
//}

void D3D11DeviceImpl::clearBackBuffer()
{
	clearRenderTarget(ERenderTarget::RT_BACK_BUFFER);

	//m_deviceContext->ClearDepthStencilView(m_depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D11DeviceImpl::present()
{
	m_swapChain->Present(m_vsync_enabled ? 1 : 0, 0);
}

VertexBuffer* D3D11DeviceImpl::createVertexBuffer(const void *pData, unsigned int elementsCount, const unsigned int stride)
{
	IVertexBufferImpl *vbImpl = new D3D11VertexBufferImpl(m_device, m_deviceContext);

	bool created = vbImpl->create(pData, elementsCount, stride);

	assert(created);

	VertexBuffer *vertexBuffer = new VertexBuffer(vbImpl);

	return vertexBuffer;
}

IndexBuffer* D3D11DeviceImpl::createIndexBuffer(const void *pData, size_t elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format)
{
	IIndexBufferImpl *ibImpl = new D3D11IndexBufferImpl(m_device, m_deviceContext);

	bool created = ibImpl->create(pData, elementsCount, stride, format);

	assert(created);

	IndexBuffer *indexBuffer = new IndexBuffer(ibImpl);

	return indexBuffer;
}


Shader* D3D11DeviceImpl::createVertexShader(const void* pByteCode, size_t ByteCodeLength)
{
	IShaderImpl *shaderImpl = new D3D11VertexShaderImpl(m_device, m_deviceContext);

	bool created = shaderImpl->create(pByteCode, ByteCodeLength);

	assert(created);

	Shader *shader = new Shader(shaderImpl);

	return shader;
}
Shader* D3D11DeviceImpl::createPixelShader(const void* pByteCode, size_t ByteCodeLength)
{
	IShaderImpl *shaderImpl = new D3D11PixelShaderImpl(m_device, m_deviceContext);

	bool created = shaderImpl->create(pByteCode, ByteCodeLength);

	assert(created);

	Shader *shader = new Shader(shaderImpl);

	return shader;
}
InputLayout* D3D11DeviceImpl::createInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength)
{
	IInputLayoutImpl *ilimpl = new D3D11InputLayoutImpl(m_device, m_deviceContext);

	bool created = ilimpl->create(layoutDesc, layoutDescLength, pByteCode, ByteCodeLength);

	assert(created);

	InputLayout* layout = new InputLayout(ilimpl);

	return layout;
}

ConstantBuffer* D3D11DeviceImpl::createConstantBuffer(unsigned int bufferSize, EConstantBufferType type)
{
	assert(bufferSize % 4 == 0);

	IContantBufferImpl *cbImpl = new D3D11ConstantBufferImpl(m_device, m_deviceContext);

	bool created = cbImpl->create(bufferSize, type);

	assert(created);

	ConstantBuffer *constantBuffer = new ConstantBuffer(cbImpl);

	return constantBuffer;
}

Texture* D3D11DeviceImpl::createTextureFromFile(wchar_t* filename)
{
	ITextureImpl *textureImpl = new D3D11TextureImpl(m_device, m_deviceContext);

	bool created = textureImpl->createFromFile(filename);

	assert(created);

	Texture *texture = new Texture(textureImpl);

	return texture;
}

Texture* D3D11DeviceImpl::createFresnelMap()
{
	ITextureImpl *textureImpl = new D3D11TextureImpl(m_device, m_deviceContext);

	const int FRESNEL_TEX_SIZE = 256;

	float blending = 16.0f;

	unsigned long* buffer = new unsigned long[FRESNEL_TEX_SIZE];
	for (int i = 0; i < FRESNEL_TEX_SIZE; i++)
	{
		float cos_a = i / (FLOAT)FRESNEL_TEX_SIZE;
		// Using water's refraction index 1.33

		XMVECTOR CosIncidentAngle = XMVectorSet(cos_a, cos_a, cos_a, cos_a);
		XMVECTOR RefractionIndex = XMVectorSet(1.33f, 1.33f, 1.33f, 1.33f);

		XMVECTOR fresnelVec = XMFresnelTerm(CosIncidentAngle, RefractionIndex);

		unsigned long fresnel = (unsigned long)(fresnelVec.m128_f32[0] * 255);

		unsigned long sky_blend = (unsigned long)(powf(1 / (1 + cos_a), blending) * 255);

		buffer[i] = (sky_blend << 8) | fresnel;
	}

	bool created = textureImpl->createTexture1D(FRESNEL_TEX_SIZE, buffer);

	if (buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}

	assert(created);

	Texture *texture = new Texture(textureImpl);

	return texture;
}

void D3D11DeviceImpl::setPrimitiveTopology(PRIMITIVE_TOPOLOGY primitiveType)
{
	switch (primitiveType)
	{
	case PRIMITIVE_TOPOLOGY::TRIANGLELIST:
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case PRIMITIVE_TOPOLOGY::TRIANGLESTRIP:
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	case PRIMITIVE_TOPOLOGY::LINELIST:
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	default:
		break;
	}
}

void D3D11DeviceImpl::setRepeatMaxAnisoSampler(sky::EShaderType shaderType, unsigned int slot)
{
	switch (shaderType)
	{
	case sky::EShaderType::STVertex:
		m_deviceContext->VSSetSamplers(slot, 1, &m_samplerRepeatMaxAniso);
		break;
	case sky::EShaderType::STHull:
		m_deviceContext->HSSetSamplers(slot, 1, &m_samplerRepeatMaxAniso);
		break;
	case sky::EShaderType::STDomain:
		m_deviceContext->DSSetSamplers(slot, 1, &m_samplerRepeatMaxAniso);
		break;
	case sky::EShaderType::STPixel:
		m_deviceContext->PSSetSamplers(slot, 1, &m_samplerRepeatMaxAniso);
		break;
	case sky::EShaderType::STGeometry:
		m_deviceContext->GSSetSamplers(slot, 1, &m_samplerRepeatMaxAniso);
		break;
	case sky::EShaderType::STCompute:
		m_deviceContext->CSSetSamplers(slot, 1, &m_samplerRepeatMaxAniso);
		break;
	default:
		break;
	}
}
void D3D11DeviceImpl::setClampLinearSampler(sky::EShaderType shaderType, unsigned int slot)
{
	switch (shaderType)
	{
	case sky::EShaderType::STVertex:
		m_deviceContext->VSSetSamplers(slot, 1, &m_samplerClampLinear);
		break;
	case sky::EShaderType::STHull:
		m_deviceContext->HSSetSamplers(slot, 1, &m_samplerClampLinear);
		break;
	case sky::EShaderType::STDomain:
		m_deviceContext->DSSetSamplers(slot, 1, &m_samplerClampLinear);
		break;
	case sky::EShaderType::STPixel:
		m_deviceContext->PSSetSamplers(slot, 1, &m_samplerClampLinear);
		break;
	case sky::EShaderType::STGeometry:
		m_deviceContext->GSSetSamplers(slot, 1, &m_samplerClampLinear);
		break;
	case sky::EShaderType::STCompute:
		m_deviceContext->CSSetSamplers(slot, 1, &m_samplerClampLinear);
		break;
	default:
		break;
	}
}

void D3D11DeviceImpl::setBackFaceCulling(bool cullBackFace)
{
	cullBackFace ? setCullBackRasterizerState() : setNoCullBackRasterizerState();
}

void D3D11DeviceImpl::setDepthState(sky::EDepthState depthState)
{
	if (depthState != _currentDepthState)
	{
		_currentDepthState = depthState;

		switch (depthState)
		{
		case sky::EDepthState::DS_DEFAULT:
			m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
			break;
		case sky::EDepthState::DS_NONE:
			m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
			break;
		case sky::EDepthState::DS_LESS_EQUAL:
			m_deviceContext->OMSetDepthStencilState(m_depthStencilStateLessEqual, 0);
			break;
		default:
			break;
		}
	}
}

void D3D11DeviceImpl::setBlendState(sky::EBlendState blendState)
{
	switch (blendState)
	{
	case sky::EBlendState::BS_OPAQUE:
		m_deviceContext->OMSetBlendState(0, 0, 0xffffffff);
		break;
	case sky::EBlendState::BS_ALPHA_BLENDING:
		{
			float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_deviceContext->OMSetBlendState(m_alphaBlendState, blendFactor, 0xffffffff);
		}
		break;
	default:
		break;
	}
}

void D3D11DeviceImpl::clearRenderTarget(ERenderTarget renderTarget)
{
	switch (renderTarget)
	{
	case ERenderTarget::RT_BACK_BUFFER:
		{
			float backBufferColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
			m_deviceContext->ClearRenderTargetView(m_backBufferRenderTarget, backBufferColor);
			m_deviceContext->ClearDepthStencilView(m_depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
		break;
	default:
		break;
	}
}

void D3D11DeviceImpl::setRenderTarget(ERenderTarget renderTarget)
{
	if (renderTarget != _currentRenderTarget)
	{
		_currentRenderTarget = renderTarget;

		switch (renderTarget)
		{
		case ERenderTarget::RT_BACK_BUFFER:
			{
				m_deviceContext->OMSetRenderTargets(1, &m_backBufferRenderTarget, m_depthStencil);
			}
			break;
			break;
		default:
			break;
		}
	}
}

Texture *D3D11DeviceImpl::getDepthMap() const
{
	return _depthMap;
}

void D3D11DeviceImpl::drawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int  BaseVertexLocation)
{
	m_deviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void D3D11DeviceImpl::drawIndexedInstanced(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int  baseVertexLocation, unsigned int startInstanceLocation)
{
	m_deviceContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void D3D11DeviceImpl::testScene()
{
	ID3D11Buffer* triangleVertBuffer;
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;
	ID3D11InputLayout* vertLayout;

	//Compile Shaders from shader file
	HRESULT hr = D3DCompileFromFile(L"2dVS.hlsl", 0, 0, "main", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VS_Buffer, 0);
	hr = D3DCompileFromFile(L"2dPS.hlsl", 0, 0, "main", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PS_Buffer, 0);

	//Create the Shader Objects
	hr = m_device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = m_device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//Set Vertex and Pixel Shaders
	m_deviceContext->VSSetShader(VS, 0, 0);
	m_deviceContext->PSSetShader(PS, 0, 0);

	struct Vertex    //Overloaded Vertex Structure
	{
		Vertex(){}
		Vertex(float x, float y, float z)
			: pos(x, y, z){}

		XMFLOAT3 pos;
	};

	//Create the vertex buffer
	Vertex v[] =
	{
		Vertex(0.0f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	//Create the Input Layout
	m_device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	m_deviceContext->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


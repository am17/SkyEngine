#include "stdafx.h"
#include "D3D11Device.h"
#include "D3DVertexBuffer.h"
#include "D3DIndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include <iostream>
#include "DDSTextureLoader.h"
#include "Log.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

using namespace std;
using namespace DirectX;

namespace sky
{
	D3D11Device::D3D11Device()
		:m_device(nullptr),
		m_deviceContext(nullptr),
		m_swapChain(nullptr),
		m_backBufferRenderTarget(nullptr),
		m_depthStencilBuffer(nullptr),
		m_depthStencilState(nullptr), 
		m_depthDisabledStencilState(nullptr),
		m_depthStencil(nullptr),
		m_cullBackRasterState(nullptr), 
		m_noCullingrasterState(nullptr),
		m_sampleState(nullptr), 
		m_cardName(nullptr), 
		m_dedicatedVideoMemory(0),
		m_vsync_enabled(false),
		m_timer(nullptr),
		m_screenWidth(0),
		m_screenHeight(0), 
		m_currentState(nullptr)
	{
	}


	D3D11Device::~D3D11Device()
	{
	}

	bool D3D11Device::create(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen)
	{
		DXGI_FORMAT pixelFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT resourceDataFormat = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;

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

		if (!createDepthStencilState())
		{
			return false;
		}

		setDepthStencilState();

		if (!createBackBufferRenderTarget(resourceDataFormat))
		{
			return false;
		}

		resetRenderTarget();

		if (!createRasterizerStates())
		{
			return false;
		}

		setCullBackRasterizerState();

		setViewPort();

		createSamplerState();

		m_timer = new Timer();

		if (!m_timer->start())
		{
			return false;
		}

		return true;
	}

	bool D3D11Device::createDeviceAndSwapChain(HWND hwnd, DXGI_FORMAT pixelFormat, bool fullscreen)
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

		stateCache.init(m_deviceContext); 

		return true;
	}

	bool D3D11Device::fillVideoCardInfo(DXGI_FORMAT pixelFormat, bool vsyncEnabled)
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

	bool D3D11Device::fillDispayModeRational(IDXGIAdapter* adapter, DXGI_FORMAT pixelFormat, bool vsyncEnabled)
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

	bool D3D11Device::fillVideoCardDescription(IDXGIAdapter* adapter)
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

	void D3D11Device::setViewPort()
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(viewport));
		viewport.Width = static_cast<float>(m_screenWidth);
		viewport.Height = static_cast<float>(m_screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		stateCache.setViewport(viewport);
	}

	bool D3D11Device::createRasterizerStates()
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

	void D3D11Device::setCullBackRasterizerState()
	{
		stateCache.setRasterizerState(m_cullBackRasterState);
	}

	bool D3D11Device::createBackBufferRenderTarget(DXGI_FORMAT resourceDataFormat)
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
		depthStencilViewDesc.Format = resourceDataFormat;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencil);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void D3D11Device::resetRenderTarget()
	{
		m_deviceContext->OMSetRenderTargets(1, &m_backBufferRenderTarget, m_depthStencil);
	}

	void D3D11Device::clearRenderTarget(IRenderTargetResources* renderTargetResources)
	{
		D3D11RenderTargetResources *d3dRenderTargetResources = static_cast<D3D11RenderTargetResources*>(renderTargetResources);

		ID3D11RenderTargetView* renderTargetView = d3dRenderTargetResources->getRenderTarget();

		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_deviceContext->ClearRenderTargetView(renderTargetView, color);

		m_deviceContext->ClearDepthStencilView(m_depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	bool D3D11Device::createDepthStencilState()
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

		return true;
	}

	void D3D11Device::setDepthStencilState()
	{
		stateCache.setDepthStencilState(m_depthStencilState, 1);
	}

	bool D3D11Device::createDepthStencilBuffer(DXGI_FORMAT resourceDataFormat)
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
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		HRESULT result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void D3D11Device::setPrimitiveTopology(PRIMITIVE_TOPOLOGY primitiveType)
	{
		switch (primitiveType)
		{
		case sky::PRIMITIVE_TOPOLOGY::TRIANGLELIST:
			stateCache.setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case sky::PRIMITIVE_TOPOLOGY::LINELIST:
			stateCache.setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		default:
			break;
		}
	}

	void D3D11Device::createSamplerState()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = m_device->CreateSamplerState(&samplerDesc, &m_sampleState);

		if (FAILED(result))
		{
			Log::write("createSamplerState ERROR");
		}
		else
		{
			stateCache.setSamplerState(EShaderType::ST_Pixel, m_sampleState, 0);
		}
	}

	void D3D11Device::turnZBuffer(bool zBufferEnabled)
	{
		stateCache.setDepthStencilState(zBufferEnabled ? m_depthStencilState : m_depthDisabledStencilState, 1);
	}

	void  D3D11Device::turnBackfaceCulling(bool cullBackface)
	{
		stateCache.setRasterizerState(cullBackface ? m_cullBackRasterState : m_noCullingrasterState);
	}

	void D3D11Device::initText()
	{
		m_font = std::make_unique<SpriteFont>(m_device, L"Fonts/couriernew.spritefont");
		m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceContext);

		m_fontPos.x = 100;
		m_fontPos.y = 100;
	}

	void D3D11Device::drawText() const
	{
		m_spriteBatch->Begin();

		const wchar_t* output = L"Hello World";

		SimpleMath::Vector2 origin = m_font->MeasureString(output) / 2.f;

		m_font->DrawString(m_spriteBatch.get(), output, m_fontPos, Colors::White, 0.f, origin);

		m_spriteBatch->End();
	}

	void D3D11Device::close()
	{
		unordered_map<DWORD, VertexInputLayout*>::iterator itr;
		for (itr = m_inputLayouts.begin(); itr != m_inputLayouts.end(); ++itr){
			if ((*itr).second){
				delete ((*itr).second);
				((*itr).second) = nullptr;
			}
		}

		m_inputLayouts.clear();

		if (m_timer)
		{
			delete m_timer;
			m_timer = nullptr;
		}

		if (m_cardName)
		{
			delete[] m_cardName; 
			m_cardName = nullptr;
		}

		if (m_swapChain)
		{
			m_swapChain->SetFullscreenState(false, nullptr);
		}

		if (m_sampleState)
		{
			m_sampleState->Release();
			m_sampleState = nullptr;
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

		if (m_backBufferRenderTarget)
		{
			m_backBufferRenderTarget->Release();
			m_backBufferRenderTarget = nullptr;
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

	void D3D11Device::beginScene()
	{
		m_timer->tick();

		float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

		m_deviceContext->ClearRenderTargetView(m_backBufferRenderTarget, color);

		m_deviceContext->ClearDepthStencilView(m_depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}

	void D3D11Device::endScene()
	{
		HRESULT hr = m_swapChain->Present(m_vsync_enabled ? 1 : 0, 0);
	}

	Timer* D3D11Device::getTimer()
	{
		return m_timer;
	}

	int D3D11Device::getScreenWidth()
	{
		return m_screenWidth;
	}
	int D3D11Device::getScreenHeight()
	{
		return m_screenHeight;
	}

	float D3D11Device::getScreenAspectRatio()
	{
		return (float)m_screenWidth / (float)m_screenHeight;
	}

	IAPIBuffer* D3D11Device::createVertexBuffer(const void *pData, size_t elementsCount, unsigned int stride)
	{
		unsigned int bufferSize = stride * elementsCount;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = bufferSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = pData;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		ID3D11Buffer *m_vertexBuffer = nullptr;
		IAPIBuffer *vertexBuffer = nullptr;

		HRESULT result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (SUCCEEDED(result))
		{
			vertexBuffer = new D3DVertexBuffer();
			vertexBuffer->setResource(m_vertexBuffer);
			vertexBuffer->setSize(bufferSize);
			vertexBuffer->setStride(stride);
			vertexBuffer->setElementsCount(elementsCount);
		}

		return vertexBuffer;
	}
	IAPIBuffer* D3D11Device::createIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride)
	{
		unsigned int bufferSize = stride * elementsCount;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = bufferSize;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = pData;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		ID3D11Buffer *m_indexBuffer = nullptr;
		IAPIBuffer *indexBuffer = nullptr;

		HRESULT result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (SUCCEEDED(result))
		{
			indexBuffer = new D3DIndexBuffer();
			indexBuffer->setResource(m_indexBuffer);
			indexBuffer->setSize(bufferSize);
			indexBuffer->setElementsCount(elementsCount);
		}

		return indexBuffer;
	}
	IAPIBuffer* D3D11Device::createConstantBuffer(unsigned int bufferSize, bool dynamic)
	{
		assert(bufferSize % 4 == 0);

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
		constantBufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		constantBufferDesc.ByteWidth = bufferSize;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		ID3D11Buffer* d2d11constantBuffer = nullptr;
		IAPIBuffer *constantBuffer = nullptr;

		HRESULT result = m_device->CreateBuffer(&constantBufferDesc, nullptr, &d2d11constantBuffer);
		if (SUCCEEDED(result))
		{
			constantBuffer = new ConstantBuffer();
			constantBuffer->setResource(d2d11constantBuffer);
			constantBuffer->setSize(bufferSize);
		}

		return constantBuffer;
	}
	IVertexShader* D3D11Device::createVertexShader(const void* pByteCode, size_t ByteCodeLength, size_t constantBufferCount, size_t vertexBufferCount)
	{
		ID3D11VertexShader* m_vertexShader = nullptr;
		IVertexShader *shader = nullptr;

		HRESULT result = m_device->CreateVertexShader(pByteCode, ByteCodeLength, nullptr, &m_vertexShader);
		if (SUCCEEDED(result))
		{
			shader = new VertexShader(pByteCode, ByteCodeLength, constantBufferCount, vertexBufferCount);
			shader->setResource(m_vertexShader);
		}

		return shader;
	}
	IPixelShader* D3D11Device::createPixelShader(const void* pByteCode, size_t ByteCodeLength, size_t constantBufferCount, size_t shaderResourceCount)
	{
		ID3D11PixelShader* m_pixelShader = nullptr;
		IPixelShader *shader = nullptr;

		HRESULT result = m_device->CreatePixelShader(pByteCode, ByteCodeLength, nullptr, &m_pixelShader);
		if (SUCCEEDED(result))
		{
			shader = new PixelShader(pByteCode, ByteCodeLength, constantBufferCount, shaderResourceCount);
			shader->setResource(m_pixelShader);
		}

		return shader;
	}

	IShaderResource *D3D11Device::createShaderResource(WCHAR* filename)
	{
		ID3D11ShaderResourceView* texture = nullptr;
		D3D11ShaderResource* shaderResource = nullptr;

		HRESULT result = CreateDDSTextureFromFile(m_device, filename, nullptr, &texture, 0, nullptr);

		if (SUCCEEDED(result))
		{
			shaderResource = new D3D11ShaderResource(texture);
		}

		return shaderResource;
	}

	IRenderTargetResources *D3D11Device::createRenderTargetResources(int textureWidth, int textureHeight)
	{
		ID3D11Texture2D* renderTargetTexture = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;
		ID3D11ShaderResourceView* shaderResourceView = nullptr;
		D3D11ShaderResource* shaderResource = nullptr;
		D3D11RenderTargetResources *renderTargetResources = nullptr;

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT result = m_device->CreateTexture2D(&textureDesc, nullptr, &renderTargetTexture);

		assert(SUCCEEDED(result));

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		result = m_device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);

		assert(SUCCEEDED(result));

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		result = m_device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
		
		assert(SUCCEEDED(result));

		shaderResource = new D3D11ShaderResource(shaderResourceView);

		renderTargetResources = new D3D11RenderTargetResources(renderTargetTexture, renderTargetView, shaderResource);

		assert(renderTargetResources != nullptr);

		return renderTargetResources;
	}

	VertexInputLayout* D3D11Device::getInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength)
	{
		VertexInputLayout* layout = nullptr;

		DWORD layoutKey = 0;
		for (size_t i = 0; i < layoutDescLength; i++)
		{
			layoutKey |= layoutDesc[i].Format | *layoutDesc[i].SemanticName | layoutDesc[i].SemanticIndex;
		}

		unordered_map<DWORD, VertexInputLayout*>::const_iterator it = m_inputLayouts.find(layoutKey);

		if (it == m_inputLayouts.end())
		{
			assert(layoutDescLength <= 32);

			D3D11_INPUT_ELEMENT_DESC elementDescs[32];

			for (size_t i = 0; i < layoutDescLength; i++)
			{
				elementDescs[i].SemanticName = layoutDesc[i].SemanticName;
				elementDescs[i].SemanticIndex = layoutDesc[i].SemanticIndex;
				elementDescs[i].InputSlot = layoutDesc[i].InputSlot;
				elementDescs[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
				elementDescs[i].InputSlotClass = layoutDesc[i].ForInstance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
				elementDescs[i].InstanceDataStepRate = layoutDesc[i].ForInstance ? 1 : 0;

				switch (layoutDesc[i].Format)
				{
				case ELEMENT_FORMAT::FLOAT2:
					elementDescs[i].Format = DXGI_FORMAT_R32G32_FLOAT;
					break;
				case ELEMENT_FORMAT::FLOAT3:
					elementDescs[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
					break;
				case ELEMENT_FORMAT::FLOAT4:
					elementDescs[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				default:
					break;
				}

			}

			ID3D11InputLayout* inputLayout = nullptr;

			HRESULT result = m_device->CreateInputLayout(elementDescs, layoutDescLength, pByteCode, ByteCodeLength, &inputLayout);
			if (SUCCEEDED(result))
			{
				layout = new VertexInputLayout();
				layout->setData(inputLayout);

				m_inputLayouts.insert(pair<DWORD, VertexInputLayout*>(layoutKey, layout));
			}
		}
		else
		{
			layout = it->second;
		}

		assert(layout != nullptr);

		return layout;
	}

	void D3D11Device::setPipelineState(D3D11PipelineState* renderTmlt)
	{
		IAStage *IAStageData = renderTmlt->getIAStageData();

		if (IAStageData)
		{
			const unsigned int bufferCount = IAStageData->getVertexBuffersCount();

			for (size_t i = 0; i < bufferCount; i++)
			{
				IAPIBuffer* vertexBuffer = IAStageData->getVertexBuffer(i);

				ID3D11Buffer* vBuffer = vertexBuffer->getResource();
				unsigned int stride = vertexBuffer->getStride();
				unsigned int offset = 0;

				stateCache.setVertexBuffer(vBuffer, i, stride, offset);
			}

			IAPIBuffer *indexBuffer = IAStageData->getIndexBuffer();

			const DXGI_FORMAT Format = (indexBuffer->getStride() == sizeof(unsigned short) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);

			stateCache.setIndexBuffer(indexBuffer->getResource(), Format, 0);

			VertexInputLayout *layout = IAStageData->getInputLayout();

			ID3D11InputLayout * layoutData = layout->getData();

			stateCache.setInputLayout(layoutData);

			PRIMITIVE_TOPOLOGY topology = IAStageData->getPrimitiveTopology();

			setPrimitiveTopology(topology);
		}

		VSStage *VSStageData = renderTmlt->getVSStageData();

		if (VSStageData)
		{
			IVertexShader *pVertexShader = VSStageData->getVertexShader();

			const unsigned int constantbufferCount = pVertexShader->getConstantBufferCount();

			for (size_t i = 0; i < constantbufferCount; i++)
			{
				ConstantBuffer *cb = VSStageData->getConstantBuffer(i);

				ID3D11Buffer* constantBuffer = cb->getResource();
				const void* pContantBufferData = cb->getData();

				if (pContantBufferData != nullptr)
				{
					m_deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, pContantBufferData, 0, 0);

					stateCache.setConstantBuffer(EShaderType::ST_Vertex, constantBuffer, i);
				}
			}

			ID3D11VertexShader* vertexShader = pVertexShader->getResource();

			stateCache.setVertexShader(vertexShader);
		}

		PSStage *PSStageData = renderTmlt->getPSStageData();

		if (PSStageData)
		{
			IPixelShader *pPixelShader = PSStageData->getPixelShader();

			const size_t shaderResorceCount = pPixelShader->getShaderResourceCount();

			for (size_t i = 0; i < shaderResorceCount; i++)
			{
				IShaderResource* shaderResorce = PSStageData->getShaderResourcer(i);

				if (shaderResorce)
				{
					D3D11ShaderResource *d3dRec = static_cast<D3D11ShaderResource *>(shaderResorce);

					stateCache.setShaderResourceView(EShaderType::ST_Pixel, d3dRec->getShaderResource(), i);
				}
				else
				{
					stateCache.setShaderResourceView(EShaderType::ST_Pixel, nullptr, i);
				}
			}

			const unsigned int constantbufferCount = pPixelShader->getConstantBufferCount();

			for (size_t i = 0; i < constantbufferCount; i++)
			{
				ConstantBuffer *cb = PSStageData->getConstantBuffer(i);

				ID3D11Buffer* constantBuffer = cb->getResource();
				const void* pContantBufferData = cb->getData();

				m_deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, pContantBufferData, 0, 0);

				stateCache.setConstantBuffer(EShaderType::ST_Pixel, constantBuffer, i);
			}

			ID3D11PixelShader* pixelShader = pPixelShader->getResource();

			stateCache.setPixelShader(pixelShader);
		}

		RSStage *RSStageData = renderTmlt->getRSStageData();
		if (RSStageData)
		{
			bool cullBackface = RSStageData->getBackfaceCulling();

			turnBackfaceCulling(cullBackface);
		}

		OMStage *OMStageData = renderTmlt->getOMStageData();

		if (OMStageData)
		{
			bool zBufferEnabled = OMStageData->getZbufferEnabled();

			turnZBuffer(zBufferEnabled);
		}
	}

	void D3D11Device::draw(SDrawCall* drawCalls, size_t callCount)
	{
		for (size_t i = 0; i < callCount; i++)
		{
			SDrawCall cmd = drawCalls[i];

			if (cmd.BeCalled)
			{
				cmd.InstanceCount == 1 ? m_deviceContext->DrawIndexed(cmd.IndexCount, cmd.StartIndexLocation, 0) : m_deviceContext->DrawIndexedInstanced(cmd.IndexCount, cmd.InstanceCount, cmd.StartIndexLocation, 0, 0);
			}
		}
	}

	void D3D11Device::setRenderTarget(IRenderTargetResources* renderTargetResources)
	{
		D3D11RenderTargetResources *d3dRenderTargetResources = static_cast<D3D11RenderTargetResources*>(renderTargetResources);

		ID3D11RenderTargetView* renderTargetView = d3dRenderTargetResources->getRenderTarget();

		m_deviceContext->OMSetRenderTargets(1, &renderTargetView, m_depthStencil);
	}

	namespace global
	{
		IDevice *g_Device = nullptr;
	}
}

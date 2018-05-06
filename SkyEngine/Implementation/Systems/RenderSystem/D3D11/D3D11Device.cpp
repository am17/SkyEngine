#include "stdafx.h"
#include "Systems\RenderSystem\D3D11\D3D11Device.h"
#include "Systems\RenderSystem\D3D11\D3D11Shader.h"
#include "Systems\RenderSystem\D3D11\D3D11SamplerState.h"
#include "Systems\RenderSystem\D3D11\D3D11ConstantBuffer.h"
#include "Systems\RenderSystem\D3D11\D3D11BlendState.h"
#include "Systems\RenderSystem\D3D11\D3D11VertexDeclaration.h"
#include "Systems\RenderSystem\D3D11\D3D11RasterizerState.h"
#include "Systems\RenderSystem\D3D11\D3D11DepthStencilState.h"
#include "Systems\RenderSystem\D3D11\D3D11VertexBuffer.h"
#include "Systems\RenderSystem\D3D11\D3D11IndexBuffer.h"
#include "Systems\RenderSystem\D3D11\D3D11ViewPort.h"
#include "Systems\RenderSystem\D3D11\D3D11Texture2D.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

D3D11Device::D3D11Device()
{
}


D3D11Device::~D3D11Device()
{
}

bool D3D11Device::Init()
{
	HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)mDXGIFactory.ReleaseAndGetAddressOf());

	if (FAILED(result)) return false;

	if (!mDirect3DDevice)
	{
		D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;

		unsigned int DeviceFlags = 0;

#if _DEBUG
		DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

		const D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		result = D3D11CreateDevice(nullptr, DriverType, nullptr, DeviceFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, mDirect3DDevice.ReleaseAndGetAddressOf(), &FeatureLevel, mD3DDeviceIMContext.ReleaseAndGetAddressOf());

		if (FAILED(result)) return false;
	}

	CreateCommonStates();

	return true;
}

Texture2D *D3D11Device::CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality)
{
	assert(width > 0 && height > 0);
	assert(createRTV != createDSV);

	D3D11Texture2D *texture = new D3D11Texture2D(width, height, multiSampleCount > 1);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	D3D11_SHADER_RESOURCE_VIEW_DESC textureSRV_desc;
	ZeroMemory(&textureSRV_desc, sizeof(textureSRV_desc));

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));

	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;

	if (createRTV)
	{
		BindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	if (createDSV)
	{
		BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	}

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = createDSV ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = multiSampleCount;
	textureDesc.SampleDesc.Quality = multiSampleQuality;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = BindFlags;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	textureSRV_desc.Format = createDSV ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM;
	textureSRV_desc.ViewDimension = multiSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	textureSRV_desc.Texture2D.MipLevels = textureDesc.MipLevels;
	textureSRV_desc.Texture2D.MostDetailedMip = 0;

	HRESULT result = mDirect3DDevice->CreateTexture2D(&textureDesc, 0, texture->Resource.ReleaseAndGetAddressOf());

	result = mDirect3DDevice->CreateShaderResourceView(texture->Resource.Get(), &textureSRV_desc, texture->ShaderResourceView.ReleaseAndGetAddressOf());
	
	assert(SUCCEEDED(result));

	if (createDSV)
	{
		DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DSVDesc.ViewDimension = multiSampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		DSVDesc.Flags = 0;
		DSVDesc.Texture2D.MipSlice = 0;

		result = mDirect3DDevice->CreateDepthStencilView(texture->Resource.Get(), &DSVDesc, texture->DepthStencilView.ReleaseAndGetAddressOf());
	}

	if (createRTV)
	{
		//D3D11_TEXTURE2D_DESC desc;
		//texture->GetDesc(&desc);
		//Width = desc.Width;
		//Height = desc.Height;

		//nullptr for all subresorces in 0 mipmap level
		result = mDirect3DDevice->CreateRenderTargetView(texture->Resource.Get(), nullptr, texture->RenderTargetView.ReleaseAndGetAddressOf());
	}

	assert(SUCCEEDED(result));

	return texture;
}

Texture2D * D3D11Device::CreateTexture2D(ID3D11Texture2D * texture, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality)
{
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	unsigned int Width = desc.Width;
	unsigned int Height = desc.Height;

	D3D11Texture2D *texture2d = new D3D11Texture2D(Width, Height, multiSampleCount > 1);

	D3D11_SHADER_RESOURCE_VIEW_DESC textureSRV_desc;
	ZeroMemory(&textureSRV_desc, sizeof(textureSRV_desc));

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));

	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;

	if (createRTV)
	{
		BindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	if (createDSV)
	{
		BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	}

	textureSRV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureSRV_desc.ViewDimension = multiSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	textureSRV_desc.Texture2D.MipLevels = desc.MipLevels;
	textureSRV_desc.Texture2D.MostDetailedMip = 0;

	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = multiSampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = 0;
	DSVDesc.Texture2D.MipSlice = 0;

	HRESULT result;
	/*if (desc.BindFlags == D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET || desc.BindFlags == D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL)
	{
		result = mDirect3DDevice->CreateShaderResourceView(texture, &textureSRV_desc, texture2d->ShaderResourceView.ReleaseAndGetAddressOf());

		assert(SUCCEEDED(result));
	}*/

	if (createDSV)
	{
		result = mDirect3DDevice->CreateDepthStencilView(texture, &DSVDesc, texture2d->DepthStencilView.ReleaseAndGetAddressOf());
	}

	if (createRTV)
	{
		//nullptr for all subresorces in 0 mipmap level
		result = mDirect3DDevice->CreateRenderTargetView(texture, nullptr, texture2d->RenderTargetView.ReleaseAndGetAddressOf());
	}

	assert(SUCCEEDED(result));

	return texture2d;
}

VertexShader * D3D11Device::CreateVertexShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11VertexShader *shader = new D3D11VertexShader();

	HRESULT hr = mDirect3DDevice->CreateVertexShader(pByteCode, ByteCodeLength, nullptr, shader->Resource.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	shader->Code = (void*)pByteCode;
	shader->ByteCodeLength = ByteCodeLength;

	return shader;
}

HullShader * D3D11Device::CreateHullShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11HullShader *shader = new D3D11HullShader();

	HRESULT hr = mDirect3DDevice->CreateHullShader(pByteCode, ByteCodeLength, nullptr, shader->Resource.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	return shader;
}

DomainShader * D3D11Device::CreateDomainShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11DomainShader *shader = new D3D11DomainShader();

	HRESULT hr = mDirect3DDevice->CreateDomainShader(pByteCode, ByteCodeLength, nullptr, shader->Resource.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	return shader;
}

PixelShader * D3D11Device::CreatePixelShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11PixelShader *shader = new D3D11PixelShader();

	HRESULT hr = mDirect3DDevice->CreatePixelShader(pByteCode, ByteCodeLength, nullptr, shader->Resource.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	return shader;
}

GeometryShader * D3D11Device::CreateGeometryShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11GeometryShader *shader = new D3D11GeometryShader();

	HRESULT hr = mDirect3DDevice->CreateGeometryShader(pByteCode, ByteCodeLength, nullptr, shader->Resource.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	return shader;
}

ComputeShader * D3D11Device::CreateComputeShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11ComputeShader *shader = new D3D11ComputeShader();

	HRESULT hr = mDirect3DDevice->CreateComputeShader(pByteCode, ByteCodeLength, nullptr, shader->Resource.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	return shader;
}

SamplerState * D3D11Device::CreateSamplerState(const SamplerStateDesc& Initializer)
{
	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	SamplerDesc.AddressU = ConvertAddressMode(Initializer.TextureAddress);;
	SamplerDesc.AddressV = ConvertAddressMode(Initializer.TextureAddress);;
	SamplerDesc.AddressW = ConvertAddressMode(Initializer.TextureAddress);;
	SamplerDesc.ComparisonFunc = ConvertCompareFunction(Initializer.ComparisonFunction);;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	SamplerDesc.BorderColor[0] = 1;
	SamplerDesc.BorderColor[1] = 1;
	SamplerDesc.BorderColor[2] = 1;
	SamplerDesc.BorderColor[3] = 1;
	SamplerDesc.MaxAnisotropy = Initializer.MaxAnisotropy;

	const bool bComparisonEnabled = Initializer.ComparisonFunction != COMPARISON_FUNCTION::NEVER;

	switch (Initializer.Filter)
	{
	case TEXTURE_FILTER::ANISOTROPIC:
		{
			if (SamplerDesc.MaxAnisotropy == 1)
			{
				SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
			else
			{
				SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;
			}
		}
		break;
	case TEXTURE_FILTER::LINEAR:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case TEXTURE_FILTER::POINT:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	default:
		break;
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerStateHandle;

	//Если такой сэмплер уже был создан, то вернется тот же указатель
	HRESULT hr = mDirect3DDevice->CreateSamplerState(&SamplerDesc, SamplerStateHandle.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	D3D11SamplerState* SState = new D3D11SamplerState();
	SState->Resource = SamplerStateHandle;

	return SState;
}

BlendState * D3D11Device::CreateBlendState()
{
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

	Microsoft::WRL::ComPtr<ID3D11BlendState> StateHandle;

	HRESULT hr = mDirect3DDevice->CreateBlendState(&blendDesc, StateHandle.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	D3D11BlendState *state = new D3D11BlendState();
	state->Resource = StateHandle;

	return state;
}

ConstantBuffer * D3D11Device::CreateConstantBuffer(unsigned int BufferSize)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;

	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = BufferSize;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;

	HRESULT hr = mDirect3DDevice->CreateBuffer(&BufferDesc, 0, Buffer.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	D3D11ConstantBuffer* CBuffer = new D3D11ConstantBuffer(BufferSize);
	CBuffer->Resource = Buffer;

	return CBuffer;
}

VertexDeclaration * D3D11Device::CreateVertexDeclaration(vector<VertexElement>& Elements)
{
	vector<D3D11_INPUT_ELEMENT_DESC> declarations;

	for (vector<VertexElement>::iterator it = Elements.begin(); it != Elements.end(); ++it)
	{
		const VertexElement& Element = *it;
		D3D11_INPUT_ELEMENT_DESC D3DElement = { 0 };
		D3DElement.InputSlot = Element.StreamIndex;
		D3DElement.AlignedByteOffset = Element.Offset;

		switch (Element.Type)
		{
		case VERTEX_ELEMENT_TYPE::FLOAT1:
			D3DElement.Format = DXGI_FORMAT_R32_FLOAT; break;
		case VERTEX_ELEMENT_TYPE::FLOAT2:
			D3DElement.Format = DXGI_FORMAT_R32G32_FLOAT; break;
		case VERTEX_ELEMENT_TYPE::FLOAT3:
			D3DElement.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case VERTEX_ELEMENT_TYPE::FLOAT4:
			D3DElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		case VERTEX_ELEMENT_TYPE::PACKEDNORMAL:
			D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
		case VERTEX_ELEMENT_TYPE::UBYTE4:
			D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UINT; break;
		case VERTEX_ELEMENT_TYPE::UBYTE4N:
			D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
		case VERTEX_ELEMENT_TYPE::COLOR:
			D3DElement.Format = DXGI_FORMAT_B8G8R8A8_UNORM; break;
		case VERTEX_ELEMENT_TYPE::SHORT2:
			D3DElement.Format = DXGI_FORMAT_R16G16_SINT; break;
		case VERTEX_ELEMENT_TYPE::SHORT4:
			D3DElement.Format = DXGI_FORMAT_R16G16B16A16_SINT; break;
		case VERTEX_ELEMENT_TYPE::SHORT2N:
			D3DElement.Format = DXGI_FORMAT_R16G16_SNORM; break;
		case VERTEX_ELEMENT_TYPE::HALF2:
			D3DElement.Format = DXGI_FORMAT_R16G16_FLOAT; break;
		case VERTEX_ELEMENT_TYPE::HALF4:
			D3DElement.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
		case VERTEX_ELEMENT_TYPE::SHORT4N:
			D3DElement.Format = DXGI_FORMAT_R16G16B16A16_SNORM; break;
		case VERTEX_ELEMENT_TYPE::USHORT2:
			D3DElement.Format = DXGI_FORMAT_R16G16_UINT; break;
		case VERTEX_ELEMENT_TYPE::USHORT4:
			D3DElement.Format = DXGI_FORMAT_R16G16B16A16_UINT; break;
		case VERTEX_ELEMENT_TYPE::USHORT2N:
			D3DElement.Format = DXGI_FORMAT_R16G16_UNORM; break;
		case VERTEX_ELEMENT_TYPE::USHORT4N:
			D3DElement.Format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
		case VERTEX_ELEMENT_TYPE::URGB10A2N:
			D3DElement.Format = DXGI_FORMAT_R10G10B10A2_UNORM; break;
		default:
			break;
		}

		D3DElement.SemanticName = Element.SemanticName; //"ATTRIBUTE";
		D3DElement.SemanticIndex = Element.AttributeIndex;
		D3DElement.InputSlotClass = Element.UseInstanceIndex ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
		D3DElement.InstanceDataStepRate = Element.UseInstanceIndex ? 1 : 0;

		declarations.push_back(D3DElement);
	}

	D3D11VertexDeclaration *vertexDeclaration = new D3D11VertexDeclaration(declarations);

	return vertexDeclaration;
}

RasterizerState * D3D11Device::CreateRasterizerState(const RasterizerDesc& Initializer)
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> StateHandle;

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.CullMode = Initializer.CullBack ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	desc.FrontCounterClockwise = TRUE;
	desc.MultisampleEnable = Initializer.MultisampleEnable;
	desc.FillMode = Initializer.WireframeMode ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

	HRESULT hr = mDirect3DDevice->CreateRasterizerState(&desc, StateHandle.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	D3D11RasterizerState* State = new D3D11RasterizerState();
	State->Resource = StateHandle;

	return State;
}

DepthStencilState * D3D11Device::CreateDepthStencilState(const DepthStencilDesc& Initializer)
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> StateHandle;

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthDesc.DepthEnable = Initializer.DepthEnable;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.StencilEnable = Initializer.StencilEnable;
	depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = mDirect3DDevice->CreateDepthStencilState(&depthDesc, StateHandle.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr));

	D3D11DepthStencilState* State = new D3D11DepthStencilState();
	State->Resource = StateHandle;

	return State;
}

VertexBuffer * D3D11Device::CreateVertexBuffer(const void * pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage)
{
	unsigned int Size = stride * elementsCount;

	assert(Size > 0);

	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_BUFFER_DESC));
	Desc.ByteWidth = Size;
	Desc.Usage = (InUsage == BUFFER_USAGE_FLAGS::ANYDYNAMIC) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.CPUAccessFlags = (InUsage == BUFFER_USAGE_FLAGS::ANYDYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;

	if (InUsage == BUFFER_USAGE_FLAGS::UNORDERED_ACCESS)
	{
		Desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	if (InUsage == BUFFER_USAGE_FLAGS::BYTE_ADDRESS_BUFFER)
	{
		Desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	}

	if (InUsage == BUFFER_USAGE_FLAGS::STREAM_OUTPUT)
	{
		Desc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
	}

	if (InUsage == BUFFER_USAGE_FLAGS::DRAW_INDIRECT)
	{
		Desc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	}

	if (InUsage == BUFFER_USAGE_FLAGS::SHADER_RESOURCE)
	{
		Desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	D3D11_SUBRESOURCE_DATA InitData;

	InitData.pSysMem = pData;
	InitData.SysMemPitch = Size;
	InitData.SysMemSlicePitch = 0;
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBufferResource;
	HRESULT hr = mDirect3DDevice->CreateBuffer(&Desc, &InitData, VertexBufferResource.ReleaseAndGetAddressOf());

	return new D3D11VertexBuffer(VertexBufferResource.Get(), Size, InUsage);
}

IndexBuffer * D3D11Device::CreateIndexBuffer(const void * pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage)
{
	unsigned int Size = stride * elementsCount;

	assert(Size > 0);

	// Describe the index buffer.
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_BUFFER_DESC));
	Desc.ByteWidth = Size;
	Desc.Usage = (InUsage == BUFFER_USAGE_FLAGS::ANYDYNAMIC) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.CPUAccessFlags = (InUsage == BUFFER_USAGE_FLAGS::ANYDYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
	Desc.MiscFlags = 0;

	if (InUsage == BUFFER_USAGE_FLAGS::UNORDERED_ACCESS)
	{
		Desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	if (InUsage == BUFFER_USAGE_FLAGS::DRAW_INDIRECT)
	{
		Desc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	}

	if (InUsage == BUFFER_USAGE_FLAGS::SHADER_RESOURCE)
	{
		Desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	// If a resource array was provided for the resource, create the resource pre-populated
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = pData;
	InitData.SysMemPitch = Size;
	InitData.SysMemSlicePitch = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBufferResource;
	HRESULT hr = mDirect3DDevice->CreateBuffer(&Desc, &InitData, IndexBufferResource.ReleaseAndGetAddressOf());

	return new D3D11IndexBuffer(IndexBufferResource.Get(), stride, Size, InUsage);
}

void D3D11Device::SetVertexBuffer(VertexBuffer * vertexBuffer, unsigned int StartSlot, unsigned int Stride, unsigned int Offset)
{
	D3D11VertexBuffer* VertexBufferImp = static_cast<D3D11VertexBuffer*>(vertexBuffer);

	mD3DDeviceIMContext->IASetVertexBuffers(StartSlot, 1, VertexBufferImp->Resource.GetAddressOf(), &Stride, &Offset);
}

void D3D11Device::SetIndexBuffer(IndexBuffer * indexBuffer)
{
	D3D11IndexBuffer* IndexBufferImpl = static_cast<D3D11IndexBuffer*>(indexBuffer);

	const DXGI_FORMAT Format = (indexBuffer->GetStride() == sizeof(unsigned short) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);

	mD3DDeviceIMContext->IASetIndexBuffer(IndexBufferImpl->Resource.Get(), Format, 0);
}

void D3D11Device::SetRenderTarget(Texture2D * renderTarget, Texture2D *depthStencil, bool clearRenderTarget, bool clearDepthStensil)
{
	ID3D11RenderTargetView *rtv = nullptr;
	ID3D11DepthStencilView *dsv = nullptr;

	if (renderTarget != nullptr)
	{
		D3D11Texture2D *texture = static_cast<D3D11Texture2D*>(renderTarget);

		rtv = texture->RenderTargetView.Get();
	}

	if (depthStencil != nullptr)
	{
		D3D11Texture2D *texture = static_cast<D3D11Texture2D*>(depthStencil);

		dsv = texture->DepthStencilView.Get();
	}

	if (clearRenderTarget)
	{
		float ClearColor[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

		mD3DDeviceIMContext->ClearRenderTargetView(rtv, ClearColor);
	}

	if (clearDepthStensil)
	{
		mD3DDeviceIMContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	mD3DDeviceIMContext->OMSetRenderTargets(1, rtv != nullptr ? &rtv : nullptr, dsv);
}

void D3D11Device::Draw(unsigned int vertexCount)
{
	mD3DDeviceIMContext->Draw(vertexCount, 0);
}

void D3D11Device::DrawIndexedPrimitive(unsigned int NumPrimitives, unsigned int StartIndex, int BaseVertexIndex)
{
	mD3DDeviceIMContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mD3DDeviceIMContext->DrawIndexed(3, StartIndex, BaseVertexIndex);
}

void D3D11Device::SetViewport(ViewPort * pViewPort)
{
	assert(pViewPort != nullptr);

	D3D11ViewPort *viewPort = static_cast<D3D11ViewPort*>(pViewPort);

	mD3DDeviceIMContext->RSSetViewports(1, viewPort->Resource.get());
}

void D3D11Device::SetBlendState(COMMON_BLEND_STATES state)
{
	const float BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (state)
	{
	case COMMON_BLEND_STATES::BS_OPAQUE:
		mD3DDeviceIMContext->OMSetBlendState(Opaque, BlendFactor, 0xFFFFFFFF);
		break;
	case COMMON_BLEND_STATES::BS_ALPHABLEND:
		mD3DDeviceIMContext->OMSetBlendState(AlphaBlend, BlendFactor, 0xFFFFFFFF);
		break;
	case COMMON_BLEND_STATES::BS_ADDITIVE:
		mD3DDeviceIMContext->OMSetBlendState(Additive, BlendFactor, 0xFFFFFFFF);
		break;
	case COMMON_BLEND_STATES::BS_NONPREMULTIPLIED:
		mD3DDeviceIMContext->OMSetBlendState(NonPremultiplied, BlendFactor, 0xFFFFFFFF);
		break;
	default:
		break;
	}
}

void D3D11Device::SetDepthStencilState(COMMON_DEPTH_STENCIL_STATES state)
{
	switch (state)
	{
	case COMMON_DEPTH_STENCIL_STATES::DSS_DEPTHNONE:
		mD3DDeviceIMContext->OMSetDepthStencilState(DepthNone, 0);
		break;
	case COMMON_DEPTH_STENCIL_STATES::DSS_DEPTHDEFAULT:
		mD3DDeviceIMContext->OMSetDepthStencilState(DepthDefault, 0);
		break;
	case COMMON_DEPTH_STENCIL_STATES::DSS_DEPTHREAD:
		mD3DDeviceIMContext->OMSetDepthStencilState(DepthRead, 0);
		break;
	default:
		break;
	}
}

void D3D11Device::SetRasterizerState(COMMON_RASTERIZER_STATES state)
{
	switch (state)
	{
	case COMMON_RASTERIZER_STATES::RS_CULLNONE:
		mD3DDeviceIMContext->RSSetState(CullNone);
		break;
	case COMMON_RASTERIZER_STATES::RS_CULLCLOCKWISE:
		mD3DDeviceIMContext->RSSetState(CullClockwise);
		break;
	case COMMON_RASTERIZER_STATES::RS_CULLCOUNTERCLOCKWISE:
		mD3DDeviceIMContext->RSSetState(CullCounterClockwise);
		break;
	case COMMON_RASTERIZER_STATES::RS_WIREFRAME:
		mD3DDeviceIMContext->RSSetState(Wireframe);
		break;
	default:
		break;
	}
}

void D3D11Device::SetSamplerState(COMMON_SAMPLER_STATES state)
{
}

D3D11_TEXTURE_ADDRESS_MODE D3D11Device::ConvertAddressMode(SAMPLER_ADDRESS_MODE AddressMode)
{
	switch (AddressMode)
	{
	case SAMPLER_ADDRESS_MODE::CLAMP:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case SAMPLER_ADDRESS_MODE::MIRROR:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case SAMPLER_ADDRESS_MODE::BORDER:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	case SAMPLER_ADDRESS_MODE::WRAP:
	default:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}

D3D11_COMPARISON_FUNC D3D11Device::ConvertCompareFunction(COMPARISON_FUNCTION ComparisonFunction)
{
	switch (ComparisonFunction)
	{
	case COMPARISON_FUNCTION::ALWAYS:
		return D3D11_COMPARISON_ALWAYS;
	case COMPARISON_FUNCTION::LESS:
		return D3D11_COMPARISON_LESS;
	case COMPARISON_FUNCTION::LESSEQUAL:
		return D3D11_COMPARISON_LESS_EQUAL;
	case COMPARISON_FUNCTION::GREATER:
		return D3D11_COMPARISON_GREATER;
	case COMPARISON_FUNCTION::GREATEREQUAL:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case COMPARISON_FUNCTION::EQUAL:
		return D3D11_COMPARISON_EQUAL;
	case COMPARISON_FUNCTION::NOTEQUAL:
		return D3D11_COMPARISON_NOT_EQUAL;
	case COMPARISON_FUNCTION::NEVER:
	default:
		return D3D11_COMPARISON_NEVER;
	}
}

void D3D11Device::CreateCommonStates()
{
	CD3D11_DEFAULT def;
	CD3D11_BLEND_DESC desc(def);

	// Opaque
	HRESULT hr = mDirect3DDevice->CreateBlendState(&desc, &Opaque);

	// AlphaBlend
	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	hr = mDirect3DDevice->CreateBlendState(&desc, &AlphaBlend);

	// Additive
	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	hr = mDirect3DDevice->CreateBlendState(&desc, &Additive);

	// NonPremultiplied
	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	hr = mDirect3DDevice->CreateBlendState(&desc, &NonPremultiplied);

	CD3D11_DEPTH_STENCIL_DESC depthDesc(def);

	// DepthDefault
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = mDirect3DDevice->CreateDepthStencilState(&depthDesc, &DepthDefault);

	// DepthRead
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = mDirect3DDevice->CreateDepthStencilState(&depthDesc, &DepthRead);

	// DepthNone
	depthDesc.DepthEnable = FALSE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = mDirect3DDevice->CreateDepthStencilState(&depthDesc, &DepthNone);


	// CullNone
	CD3D11_RASTERIZER_DESC restDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE,
	FALSE /* FrontCounterClockwise */,
	D3D11_DEFAULT_DEPTH_BIAS,
	D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
	D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
	TRUE /* DepthClipEnable */,
	FALSE /* ScissorEnable */,
	TRUE /* MultisampleEnable */,
	FALSE /* AntialiasedLineEnable */);

	hr = mDirect3DDevice->CreateRasterizerState(&restDesc, &CullNone);

	// CullClockwise
	CD3D11_RASTERIZER_DESC restDesc2(D3D11_FILL_SOLID, D3D11_CULL_FRONT,
	FALSE /* FrontCounterClockwise */,
	D3D11_DEFAULT_DEPTH_BIAS,
	D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
	D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
	TRUE /* DepthClipEnable */,
	FALSE /* ScissorEnable */,
	TRUE /* MultisampleEnable */,
	FALSE /* AntialiasedLineEnable */);

	hr = mDirect3DDevice->CreateRasterizerState(&restDesc2, &CullClockwise);

	// CullCounterClockwise
	CD3D11_RASTERIZER_DESC restDesc3(D3D11_FILL_SOLID, D3D11_CULL_BACK,
	FALSE /* FrontCounterClockwise */,
	D3D11_DEFAULT_DEPTH_BIAS,
	D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
	D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
	TRUE /* DepthClipEnable */,
	FALSE /* ScissorEnable */,
	TRUE /* MultisampleEnable */,
	FALSE /* AntialiasedLineEnable */);

	hr = mDirect3DDevice->CreateRasterizerState(&restDesc3, &CullCounterClockwise);

	// Wireframe
	CD3D11_RASTERIZER_DESC restDesc4(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE,
	FALSE /* FrontCounterClockwise */,
	D3D11_DEFAULT_DEPTH_BIAS,
	D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
	D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
	TRUE /* DepthClipEnable */,
	FALSE /* ScissorEnable */,
	TRUE /* MultisampleEnable */,
	FALSE /* AntialiasedLineEnable */);

	hr = mDirect3DDevice->CreateRasterizerState(&restDesc4, &Wireframe);

	float maxAnisotropy = 16;
	const float border[4] = { 0.f, 0.f, 0.f, 0.f };

	// PointWrap
	CD3D11_SAMPLER_DESC samplerDesc1(D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		0.f, maxAnisotropy, D3D11_COMPARISON_NEVER, border, 0.f, FLT_MAX);

	hr = mDirect3DDevice->CreateSamplerState(&samplerDesc1, &PointWrap);

	// PointClamp
	CD3D11_SAMPLER_DESC samplerDesc2(D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP,
		0.f, maxAnisotropy, D3D11_COMPARISON_NEVER, border, 0.f, FLT_MAX);

	hr = mDirect3DDevice->CreateSamplerState(&samplerDesc2, &PointClamp);

	// LinearWrap
	CD3D11_SAMPLER_DESC samplerDesc3(D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		0.f, maxAnisotropy, D3D11_COMPARISON_NEVER, border, 0.f, FLT_MAX);

	hr = mDirect3DDevice->CreateSamplerState(&samplerDesc3, &LinearWrap);

	// LinearClamp
	CD3D11_SAMPLER_DESC samplerDesc4(D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP,
		0.f, maxAnisotropy, D3D11_COMPARISON_NEVER, border, 0.f, FLT_MAX);

	hr = mDirect3DDevice->CreateSamplerState(&samplerDesc4, &LinearClamp);

	// AnisotropicWrap
	CD3D11_SAMPLER_DESC samplerDesc5(D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		0.f, maxAnisotropy, D3D11_COMPARISON_NEVER, border, 0.f, FLT_MAX);

	hr = mDirect3DDevice->CreateSamplerState(&samplerDesc5, &AnisotropicWrap);

	// AnisotropicClamp
	CD3D11_SAMPLER_DESC samplerDesc6(D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP,
		0.f, maxAnisotropy, D3D11_COMPARISON_NEVER, border, 0.f, FLT_MAX);

	hr = mDirect3DDevice->CreateSamplerState(&samplerDesc6, &AnisotropicClamp);
}

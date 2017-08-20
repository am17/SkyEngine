#include "stdafx.h"
#include "D3D11Device.h"
#include "D3D11Shader.h"
#include "D3D11SamplerState.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11BlendState.h"
#include "D3D11VertexDeclaration.h"

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

	return true;
}

void *D3D11Device::CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality)
{
	assert(width > 0 && height > 0);
	assert(createRTV != createDSV);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> TextureResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> textureRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> textureDSV;

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

	textureSRV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureSRV_desc.ViewDimension = multiSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	textureSRV_desc.Texture2D.MipLevels = textureDesc.MipLevels;
	textureSRV_desc.Texture2D.MostDetailedMip = 0;

	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = multiSampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = 0;
	DSVDesc.Texture2D.MipSlice = 0;

	HRESULT result = mDirect3DDevice->CreateTexture2D(&textureDesc, 0, TextureResource.ReleaseAndGetAddressOf());
	result = mDirect3DDevice->CreateShaderResourceView(TextureResource.Get(), &textureSRV_desc, textureSRV.ReleaseAndGetAddressOf());
	
	assert(SUCCEEDED(result));

	if (createDSV)
	{
		result = mDirect3DDevice->CreateDepthStencilView(TextureResource.Get(), &DSVDesc, textureDSV.ReleaseAndGetAddressOf());
	}

	if (createRTV)
	{
		result = mDirect3DDevice->CreateRenderTargetView(TextureResource.Get(), nullptr, textureRTV.ReleaseAndGetAddressOf());
	}

	assert(SUCCEEDED(result));

	return TextureResource.Get();
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

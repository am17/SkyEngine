#include "stdafx.h"
#include "D3D11Device.h"
#include "skyMath.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

D3D11Device::D3D11Device()
{
}


D3D11Device::~D3D11Device()
{
}

void D3D11Device::init()
{
	initD3DDevice();
}

void D3D11Device::SetRenderTargets(const RenderTargetsInfo& renderTargetsInfo)
{
	bool bTargetChanged = false;

	const RenderTargetViewImpl* NewRenderTargetsRHI = renderTargetsInfo.RenderTargets;

	D3D11TextureBase* NewDepthStencilTarget = nullptr;
	if (renderTargetsInfo.DepthStencilRenderTarget)
	{
		NewDepthStencilTarget = static_cast<D3D11TextureBase*>((D3D11TextureBase*)renderTargetsInfo.DepthStencilRenderTarget->Texture);
	}

	ID3D11DepthStencilView* DepthStencilView = nullptr;
	if (NewDepthStencilTarget)
	{
		DepthStencilView = NewDepthStencilTarget->getDepthStencilView(0);
	}

	if (CurrentDepthStencilTarget.Get() != DepthStencilView)
	{
		CurrentDepthTexture = shared_ptr<D3D11TextureBase>(NewDepthStencilTarget);
		CurrentDepthStencilTarget = DepthStencilView;
		bTargetChanged = true;
	}

	ID3D11RenderTargetView* NewRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	for (size_t RenderTargetIndex = 0; RenderTargetIndex < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; RenderTargetIndex++)
	{
		ID3D11RenderTargetView* RenderTargetView = nullptr;
		if (RenderTargetIndex < renderTargetsInfo.RenderTargetsCount && NewRenderTargetsRHI[RenderTargetIndex].Texture != nullptr)
		{
			int RTMipIndex = NewRenderTargetsRHI[RenderTargetIndex].MipIndex;
			int RTSliceIndex = NewRenderTargetsRHI[RenderTargetIndex].ArraySliceIndex;

			D3D11TextureBase* NewRenderTarget = static_cast<D3D11TextureBase*>((D3D11TextureBase*)NewRenderTargetsRHI[RenderTargetIndex].Texture);

			if (NewRenderTarget)
			{
				RenderTargetView = NewRenderTarget->getRenderTargetView(RTMipIndex, RTSliceIndex);
			}
		}

		NewRenderTargetViews[RenderTargetIndex] = RenderTargetView;

		if (CurrentRenderTargets[RenderTargetIndex].Get() != RenderTargetView)
		{
			CurrentRenderTargets[RenderTargetIndex] = RenderTargetView;
			bTargetChanged = true;
		}
	}

	if (bTargetChanged)
	{
		ID3D11RenderTargetView* RTArray[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		for (size_t RenderTargetIndex = 0; RenderTargetIndex < renderTargetsInfo.RenderTargetsCount; ++RenderTargetIndex)
		{
			RTArray[RenderTargetIndex] = CurrentRenderTargets[RenderTargetIndex].Get();
		}

		m_direct3DDeviceIMContext->OMSetRenderTargets(renderTargetsInfo.RenderTargetsCount, RTArray, CurrentDepthStencilTarget.Get());
	}

	if (NewRenderTargetViews[0])
	{
		assert(0 < renderTargetsInfo.RenderTargetsCount && NewRenderTargetsRHI[0].Texture != nullptr);
		RTVDesc RTTDesc = GetRenderTargetViewDesc(NewRenderTargetViews[0]);
		SetViewport(0, 0, 0.0f, RTTDesc.Width, RTTDesc.Height, 1.0f);
	}
	else if (DepthStencilView)
	{
		ComPtr<ID3D11Texture2D> DepthTargetTexture;
		DepthStencilView->GetResource((ID3D11Resource**)DepthTargetTexture.ReleaseAndGetAddressOf());

		D3D11_TEXTURE2D_DESC DTTDesc;
		DepthTargetTexture->GetDesc(&DTTDesc);
		SetViewport(0, 0, 0.0f, DTTDesc.Width, DTTDesc.Height, 1.0f);
	}
}

void D3D11Device::SetViewport(unsigned int MinX, unsigned int MinY, float MinZ, unsigned int MaxX, unsigned int MaxY, float MaxZ)
{
	D3D11_VIEWPORT Viewport = { (float)MinX, (float)MinY, (float)MaxX - MinX, (float)MaxY - MinY, MinZ, MaxZ };

	m_direct3DDeviceIMContext->RSSetViewports(1, &Viewport);
}

void D3D11Device::DrawPrimitive(PRIMITIVE_TOPOLOGY PrimitiveType, unsigned int BaseVertexIndex, unsigned int NumPrimitives, unsigned int NumInstances)
{
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = GetD3D11PrimitiveType(PrimitiveType, bUsingTessellation);
	if (CurrentPrimitiveTopology != PrimitiveTopology)
	{
		CurrentPrimitiveTopology = PrimitiveTopology;
		m_direct3DDeviceIMContext->IASetPrimitiveTopology(PrimitiveTopology);
	}

	//CommitGraphicsResourceTables();
	//CommitNonComputeShaderConstants();

	unsigned int VertexCount = GetVertexCountForPrimitiveCount(NumPrimitives, PrimitiveType);

	if (NumInstances > 1)
	{
		m_direct3DDeviceIMContext->DrawInstanced(VertexCount, NumInstances, BaseVertexIndex, 0);
	}
	else
	{
		m_direct3DDeviceIMContext->Draw(VertexCount, BaseVertexIndex);
	}
}

void D3D11Device::DrawIndexedPrimitive(IndexBufferImpl *IndexBuffer, PRIMITIVE_TOPOLOGY PrimitiveType, int BaseVertexIndex, unsigned int FirstInstance, unsigned int NumVertices, unsigned int StartIndex, unsigned int NumPrimitives, unsigned int NumInstances)
{
	D3D11IndexBuffer* IndexBufferImpl = static_cast<D3D11IndexBuffer*>(IndexBuffer);

	assert(NumPrimitives > 0);

	/*CommitGraphicsResourceTables();//bind SRV, SAMPLERS, 
	CommitNonComputeShaderConstants();//constant buffers bind*/

	size_t SizeFormat = sizeof(DXGI_FORMAT);
	const DXGI_FORMAT Format = (IndexBuffer->getStride() == sizeof(unsigned short) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);

	unsigned int IndexCount = GetVertexCountForPrimitiveCount(NumPrimitives, PrimitiveType);

	assert((StartIndex + IndexCount) * IndexBuffer->getStride() <= IndexBuffer->getSize());

	m_direct3DDeviceIMContext->IASetIndexBuffer(IndexBufferImpl->Resource.Get(), Format, 0);

	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = GetD3D11PrimitiveType(PrimitiveType, bUsingTessellation);
	if (CurrentPrimitiveTopology != PrimitiveTopology)
	{
		CurrentPrimitiveTopology = PrimitiveTopology;
		m_direct3DDeviceIMContext->IASetPrimitiveTopology(PrimitiveTopology);
	}

	if (NumInstances > 1 || FirstInstance != 0)
	{
		m_direct3DDeviceIMContext->DrawIndexedInstanced(IndexCount, NumInstances, StartIndex, BaseVertexIndex, FirstInstance);
	}
	else
	{
		m_direct3DDeviceIMContext->DrawIndexed(IndexCount, StartIndex, BaseVertexIndex);
	}
}

SamplerStateImpl * D3D11Device::CreateSamplerState(const SamplerStateDesc& Initializer)
{
	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	SamplerDesc.AddressU = ConvertAddressMode(Initializer.AddressU);
	SamplerDesc.AddressV = ConvertAddressMode(Initializer.AddressV);
	SamplerDesc.AddressW = ConvertAddressMode(Initializer.AddressW);
	SamplerDesc.MipLODBias = Initializer.MipBias;
	SamplerDesc.MaxAnisotropy = ComputeAnisotropyRT(Initializer.MaxAnisotropy);
	SamplerDesc.MinLOD = Initializer.MinMipLevel;
	SamplerDesc.MaxLOD = Initializer.MaxMipLevel;

	const bool bComparisonEnabled = Initializer.SamplerComparisonFunction != SAMPLER_COMPARE_FUNCTION::NEVER;
	switch (Initializer.Filter)
	{
	case SAMPLER_FILTER::ANISOTROPICLINEAR:
	case SAMPLER_FILTER::ANISOTROPICPOINT:
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
	case SAMPLER_FILTER::TRILINEAR:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case SAMPLER_FILTER::BILINEAR:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case SAMPLER_FILTER::POINT:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	default:
		break;
	}

	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.ComparisonFunc = ConvertSamplerCompareFunction(Initializer.SamplerComparisonFunction);

	ComPtr<ID3D11SamplerState> SamplerStateHandle;

	//Если такой сэмплер уже был создан, то вернется тот же указатель
	HRESULT hr = m_direct3DDevice->CreateSamplerState(&SamplerDesc, SamplerStateHandle.ReleaseAndGetAddressOf());

	D3D11SamplerState* SamplerState = new D3D11SamplerState();
	SamplerState->Resource = SamplerStateHandle;

	/*D3D11SamplerState* Found = SamplerStateCache.find(SamplerStateHandle.Get())->second;
	if (Found)
	{
		return Found;
	}*/

	SamplerStateCache.insert(pair<ID3D11SamplerState*, D3D11SamplerState*>(SamplerStateHandle.Get(), SamplerState));

	return SamplerState;
}

RasterizerStateImpl * D3D11Device::CreateRasterizerState(const RasterizerStateDesc & Initializer)
{
	D3D11_RASTERIZER_DESC RasterizerDesc;
	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	RasterizerDesc.CullMode = ConvertCullMode(Initializer.CullMode);
	RasterizerDesc.FillMode = ConvertFillMode(Initializer.FillMode);
	RasterizerDesc.SlopeScaledDepthBias = Initializer.SlopeScaleDepthBias;
	RasterizerDesc.FrontCounterClockwise = true;
	RasterizerDesc.DepthBias = SkyMath::FloorToInt(Initializer.DepthBias * (float)(1 << 24));
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = Initializer.bAllowMSAA;
	RasterizerDesc.ScissorEnable = true;

	D3D11RasterizerState* RasterizerState = new D3D11RasterizerState();
	HRESULT hr = m_direct3DDevice->CreateRasterizerState(&RasterizerDesc, RasterizerState->Resource.ReleaseAndGetAddressOf());
	return RasterizerState;
}

DepthStencilStateImpl * D3D11Device::CreateDepthStencilState(const DepthStencilStateDesc & Initializer)
{
	D3D11DepthStencilState* DepthStencilState = new D3D11DepthStencilState();

	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// depth настройка
	DepthStencilDesc.DepthEnable = Initializer.DepthTest != COMPARE_FUNCTION::ALWAYS || Initializer.bEnableDepthWrite;
	DepthStencilDesc.DepthWriteMask = Initializer.bEnableDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthStencilDesc.DepthFunc = ConvertCompareFunction(Initializer.DepthTest);

	// stencil part
	DepthStencilDesc.StencilEnable = Initializer.bEnableFrontFaceStencil || Initializer.bEnableBackFaceStencil;
	DepthStencilDesc.StencilReadMask = Initializer.StencilReadMask;
	DepthStencilDesc.StencilWriteMask = Initializer.StencilWriteMask;
	DepthStencilDesc.FrontFace.StencilFunc = ConvertCompareFunction(Initializer.FrontFaceStencilTest);
	DepthStencilDesc.FrontFace.StencilFailOp = ConvertStencilOp(Initializer.FrontFaceStencilFailStencilOp);
	DepthStencilDesc.FrontFace.StencilDepthFailOp = ConvertStencilOp(Initializer.FrontFaceDepthFailStencilOp);
	DepthStencilDesc.FrontFace.StencilPassOp = ConvertStencilOp(Initializer.FrontFacePassStencilOp);

	if (Initializer.bEnableBackFaceStencil)
	{
		DepthStencilDesc.BackFace.StencilFunc = ConvertCompareFunction(Initializer.BackFaceStencilTest);
		DepthStencilDesc.BackFace.StencilFailOp = ConvertStencilOp(Initializer.BackFaceStencilFailStencilOp);
		DepthStencilDesc.BackFace.StencilDepthFailOp = ConvertStencilOp(Initializer.BackFaceDepthFailStencilOp);
		DepthStencilDesc.BackFace.StencilPassOp = ConvertStencilOp(Initializer.BackFacePassStencilOp);
	}
	else
	{
		DepthStencilDesc.BackFace = DepthStencilDesc.FrontFace;
	}

	//const bool bStencilOpIsKeep =
	//	Initializer.FrontFaceStencilFailStencilOp == STENCIL_OP::KEEP
	//	&& Initializer.FrontFaceDepthFailStencilOp == STENCIL_OP::KEEP
	//	&& Initializer.FrontFacePassStencilOp == STENCIL_OP::KEEP
	//	&& Initializer.BackFaceStencilFailStencilOp == STENCIL_OP::KEEP
	//	&& Initializer.BackFaceDepthFailStencilOp == STENCIL_OP::KEEP
	//	&& Initializer.BackFacePassStencilOp == STENCIL_OP::KEEP;

	//const bool bMayWriteStencil = Initializer.StencilWriteMask != 0 && !bStencilOpIsKeep;
	//DepthStencilState->AccessType.SetDepthStencilWrite(Initializer.bEnableDepthWrite, bMayWriteStencil);

	HRESULT hr = m_direct3DDevice->CreateDepthStencilState(&DepthStencilDesc, DepthStencilState->Resource.ReleaseAndGetAddressOf());

	return DepthStencilState;
}

BlendStateImpl * D3D11Device::CreateBlendState(const BlendStateDesc & Initializer)
{
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));

	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.IndependentBlendEnable = Initializer.bUseIndependentRenderTargetBlendStates;

	for (unsigned int RenderTargetIndex = 0; RenderTargetIndex < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++RenderTargetIndex)
	{
		//const BlendStateDesc::FRenderTarget& RenderTargetInitializer = Initializer.RenderTargets[RenderTargetIndex];

		D3D11_RENDER_TARGET_BLEND_DESC& RenderTarget = BlendDesc.RenderTarget[RenderTargetIndex];

		RenderTarget.BlendEnable = false;// RenderTargetInitializer.ColorBlendOp != BO_Add || RenderTargetInitializer.ColorDestBlend != BF_Zero || RenderTargetInitializer.ColorSrcBlend != BF_One || RenderTargetInitializer.AlphaBlendOp != BO_Add || RenderTargetInitializer.AlphaDestBlend != BF_Zero || RenderTargetInitializer.AlphaSrcBlend != BF_One;

		RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;// TranslateBlendOp(RenderTargetInitializer.ColorBlendOp);
		RenderTarget.SrcBlend = D3D11_BLEND_ONE;// TranslateBlendFactor(RenderTargetInitializer.ColorSrcBlend);
		RenderTarget.DestBlend = D3D11_BLEND_ZERO;// TranslateBlendFactor(RenderTargetInitializer.ColorDestBlend);
		RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;// TranslateBlendOp(RenderTargetInitializer.AlphaBlendOp);
		RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;// TranslateBlendFactor(RenderTargetInitializer.AlphaSrcBlend);
		RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;// TranslateBlendFactor(RenderTargetInitializer.AlphaDestBlend);

		RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;// ((RenderTargetInitializer.ColorWriteMask & CW_RED) ? D3D11_COLOR_WRITE_ENABLE_RED : 0) | ((RenderTargetInitializer.ColorWriteMask & CW_GREEN) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0) | ((RenderTargetInitializer.ColorWriteMask & CW_BLUE) ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0) | ((RenderTargetInitializer.ColorWriteMask & CW_ALPHA) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0);
	}

	D3D11BlendState* BlendState = new D3D11BlendState();
	HRESULT hr = m_direct3DDevice->CreateBlendState(&BlendDesc, BlendState->Resource.ReleaseAndGetAddressOf());
	return BlendState;
}

VertexShaderImpl * D3D11Device::CreateVertexShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11VertexShader* Shader = new D3D11VertexShader();

	HRESULT hr = m_direct3DDevice->CreateVertexShader(pByteCode, ByteCodeLength, nullptr, Shader->Resource.ReleaseAndGetAddressOf());

	Shader->Code = (void*)pByteCode;
	Shader->ByteCodeLength = ByteCodeLength;

	return Shader;
}

HullShaderImpl * D3D11Device::CreateHullShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11HullShader* Shader = new D3D11HullShader();

	HRESULT hr = m_direct3DDevice->CreateHullShader(pByteCode, ByteCodeLength, nullptr, Shader->Resource.ReleaseAndGetAddressOf());

	return Shader;
}

DomainShaderImpl * D3D11Device::CreateDomainShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11DomainShader* Shader = new D3D11DomainShader();

	HRESULT hr = m_direct3DDevice->CreateDomainShader(pByteCode, ByteCodeLength, nullptr, Shader->Resource.ReleaseAndGetAddressOf());

	return Shader;
}

PixelShaderImpl * D3D11Device::CreatePixelShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11PixelShader* Shader = new D3D11PixelShader();

	HRESULT hr = m_direct3DDevice->CreatePixelShader(pByteCode, ByteCodeLength, nullptr, Shader->Resource.ReleaseAndGetAddressOf());

	return Shader;
}

GeometryShaderImpl * D3D11Device::CreateGeometryShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11GeometryShader* Shader = new D3D11GeometryShader();

	HRESULT hr = m_direct3DDevice->CreateGeometryShader(pByteCode, ByteCodeLength, nullptr, Shader->Resource.ReleaseAndGetAddressOf());

	return Shader;
}

ComputeShaderImpl * D3D11Device::CreateComputeShader(const void * pByteCode, size_t ByteCodeLength)
{
	D3D11ComputeShader* Shader = new D3D11ComputeShader();

	HRESULT hr = m_direct3DDevice->CreateComputeShader(pByteCode, ByteCodeLength, nullptr, Shader->Resource.ReleaseAndGetAddressOf());

	return Shader;
}

IndexBufferImpl * D3D11Device::CreateIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage)
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

	ComPtr<ID3D11Buffer> IndexBufferResource;
	HRESULT hr = m_direct3DDevice->CreateBuffer(&Desc, &InitData, IndexBufferResource.ReleaseAndGetAddressOf());

	return new D3D11IndexBuffer(IndexBufferResource.Get(), stride, Size, InUsage);
}

void * D3D11Device::LockIndexBuffer(IndexBufferImpl * IndexBuffer, unsigned int Offset, unsigned int Size, RESOURCE_LOCK_MODE LockMode)
{
	D3D11IndexBuffer* IndexBufferImp = static_cast<D3D11IndexBuffer*>(IndexBuffer);

	// If this resource is bound to the device, unbind it
	//ConditionalClearShaderResource(IndexBuffer);

	D3D11_BUFFER_DESC Desc;
	IndexBufferImp->Resource->GetDesc(&Desc);
	const bool IsDynamic = (Desc.Usage == D3D11_USAGE_DYNAMIC);

	void* pData = nullptr;
	if (IsDynamic)
	{
		assert(LockMode == RESOURCE_LOCK_MODE::WRITEONLY);
		D3D11_MAPPED_SUBRESOURCE MappedSubresource;

		HRESULT hr = m_direct3DDeviceIMContext->Map(IndexBufferImp->Resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);

		pData = MappedSubresource.pData;
	}
	else
	{
		if (LockMode == RESOURCE_LOCK_MODE::READONLY)
		{
			D3D11_BUFFER_DESC StagingBufferDesc;
			ZeroMemory(&StagingBufferDesc, sizeof(D3D11_BUFFER_DESC));
			StagingBufferDesc.ByteWidth = Size;
			StagingBufferDesc.Usage = D3D11_USAGE_STAGING;
			StagingBufferDesc.BindFlags = 0;
			StagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			StagingBufferDesc.MiscFlags = 0;
			ComPtr<ID3D11Buffer> StagingIndexBuffer;
			HRESULT hr = m_direct3DDevice->CreateBuffer(&StagingBufferDesc, NULL, StagingIndexBuffer.ReleaseAndGetAddressOf());

			m_direct3DDeviceIMContext->CopyResource(StagingIndexBuffer.Get(), IndexBufferImp->Resource.Get());

			D3D11_MAPPED_SUBRESOURCE MappedSubresource;
			hr = m_direct3DDeviceIMContext->Map(StagingIndexBuffer.Get(), 0, D3D11_MAP_READ, 0, &MappedSubresource);
			pData = MappedSubresource.pData;
		}
		else
		{
			pData = _aligned_malloc(Desc.ByteWidth, 16);
		}
	}

	//OutstandingLocks.Add(LockedKey, LockedData);

	return pData;
}

void D3D11Device::UnlockIndexBuffer(IndexBufferImpl * IndexBuffer)
{
	D3D11IndexBuffer* IndexBufferImp = static_cast<D3D11IndexBuffer*>(IndexBuffer);

	D3D11_BUFFER_DESC Desc;
	IndexBufferImp->Resource->GetDesc(&Desc);
	const bool IsDynamic = (Desc.Usage == D3D11_USAGE_DYNAMIC);

	// Find the outstanding lock for this IB.
	//FD3D11LockedKey LockedKey(IndexBuffer->Resource);
	//FD3D11LockedData* LockedData = OutstandingLocks.Find(LockedKey);
	//assert(LockedData);

	if (IsDynamic)
	{
		m_direct3DDeviceIMContext->Unmap(IndexBufferImp->Resource.Get(), 0);
	}
	else
	{
		/*if (LockedData->StagingResource)
		{
			ID3D11Buffer* StagingBuffer = (ID3D11Buffer*)LockedData->StagingResource.GetReference();
			m_direct3DDeviceIMContext->Unmap(StagingBuffer, 0);
		}
		else
		{
			m_direct3DDeviceIMContext->UpdateSubresource(IndexBufferImp->Resource.Get(), LockedKey.Subresource, NULL, LockedData->GetData(), LockedData->Pitch, 0);

			LockedData->FreeData();
		}*/
	}

	//OutstandingLocks.Remove(LockedKey);
}

VertexBufferImpl * D3D11Device::CreateVertexBuffer(const void * pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage)
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

	ComPtr<ID3D11Buffer> VertexBufferResource;
	HRESULT hr = m_direct3DDevice->CreateBuffer(&Desc, &InitData, VertexBufferResource.ReleaseAndGetAddressOf());

	return new D3D11VertexBuffer(VertexBufferResource.Get(), Size, InUsage);
}

void * D3D11Device::LockVertexBuffer(VertexBufferImpl * VertexBuffer, unsigned int Offset, unsigned int Size, RESOURCE_LOCK_MODE LockMode)
{
	D3D11VertexBuffer* VertexBufferImp = static_cast<D3D11VertexBuffer*>(VertexBuffer);

	// If this resource is bound to the device, unbind it
	//ConditionalClearShaderResource(IndexBuffer);

	D3D11_BUFFER_DESC Desc;
	VertexBufferImp->Resource->GetDesc(&Desc);
	const bool IsDynamic = (Desc.Usage == D3D11_USAGE_DYNAMIC);

	void* pData = nullptr;
	if (IsDynamic)
	{
		assert(LockMode == RESOURCE_LOCK_MODE::WRITEONLY);
		D3D11_MAPPED_SUBRESOURCE MappedSubresource;

		HRESULT hr = m_direct3DDeviceIMContext->Map(VertexBufferImp->Resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource);

		pData = MappedSubresource.pData;
	}
	else
	{
		if (LockMode == RESOURCE_LOCK_MODE::READONLY)
		{
			D3D11_BUFFER_DESC StagingBufferDesc;
			ZeroMemory(&StagingBufferDesc, sizeof(D3D11_BUFFER_DESC));
			StagingBufferDesc.ByteWidth = Size;
			StagingBufferDesc.Usage = D3D11_USAGE_STAGING;
			StagingBufferDesc.BindFlags = 0;
			StagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			StagingBufferDesc.MiscFlags = 0;
			ComPtr<ID3D11Buffer> StagingVertexBuffer;
			HRESULT hr = m_direct3DDevice->CreateBuffer(&StagingBufferDesc, NULL, StagingVertexBuffer.ReleaseAndGetAddressOf());

			D3D11_BOX SourceBox;
			SourceBox.left = Offset;
			SourceBox.right = Size;
			SourceBox.top = SourceBox.front = 0;
			SourceBox.bottom = SourceBox.back = 1;
			m_direct3DDeviceIMContext->CopySubresourceRegion(StagingVertexBuffer.Get(), 0, 0, 0, 0, VertexBufferImp->Resource.Get(), 0, &SourceBox);

			D3D11_MAPPED_SUBRESOURCE MappedSubresource;
			hr = m_direct3DDeviceIMContext->Map(StagingVertexBuffer.Get(), 0, D3D11_MAP_READ, 0, &MappedSubresource);
			pData = MappedSubresource.pData;
		}
		else
		{
			pData = _aligned_malloc(Desc.ByteWidth, 16);
		}
	}

	//OutstandingLocks.Add(LockedKey, LockedData);

	return pData;
}

void D3D11Device::UnlockVertexBuffer(VertexBufferImpl * VertexBuffer)
{
	D3D11VertexBuffer* VertexBufferImp = static_cast<D3D11VertexBuffer*>(VertexBuffer);

	D3D11_BUFFER_DESC Desc;
	VertexBufferImp->Resource->GetDesc(&Desc);
	const bool bIsDynamic = (Desc.Usage == D3D11_USAGE_DYNAMIC);

	// Find the outstanding lock for this VB.
	//FD3D11LockedKey LockedKey(VertexBuffer->Resource);
	//FD3D11LockedData* LockedData = OutstandingLocks.Find(LockedKey);
	//assert(LockedData);

	if (bIsDynamic)
	{
		m_direct3DDeviceIMContext->Unmap(VertexBufferImp->Resource.Get(), 0);
	}
	else
	{
		/*if (LockedData->StagingResource)
		{
			ID3D11Buffer* StagingBuffer = (ID3D11Buffer*)LockedData->StagingResource.GetReference();
			m_direct3DDeviceIMContext->Unmap(StagingBuffer, 0);
		}
		else
		{
			m_direct3DDeviceIMContext->UpdateSubresource(VertexBufferImp->Resource.Get(), LockedKey.Subresource, NULL, LockedData->GetData(), LockedData->Pitch, 0);

			LockedData->FreeData();
		}*/
	}

	//OutstandingLocks.Remove(LockedKey);
}

void D3D11Device::CopyVertexBuffer(VertexBufferImpl * SourceBuffer, VertexBufferImpl * DestBuffer)
{
	D3D11VertexBuffer* SourceBufferImp = static_cast<D3D11VertexBuffer*>(SourceBuffer);
	D3D11VertexBuffer* DestBufferImp = static_cast<D3D11VertexBuffer*>(DestBuffer);

	D3D11_BUFFER_DESC SourceBufferDesc;
	SourceBufferImp->Resource->GetDesc(&SourceBufferDesc);

	D3D11_BUFFER_DESC DestBufferDesc;
	DestBufferImp->Resource->GetDesc(&DestBufferDesc);

	assert(SourceBufferDesc.ByteWidth == DestBufferDesc.ByteWidth);

	m_direct3DDeviceIMContext->CopyResource(DestBufferImp->Resource.Get(), SourceBufferImp->Resource.Get());
}

void D3D11Device::SetVertexBuffer(VertexBufferImpl * VertexBuffer, unsigned int StartSlot, unsigned int Stride, unsigned int Offset)
{
	D3D11VertexBuffer* VertexBufferImp = static_cast<D3D11VertexBuffer*>(VertexBuffer);

	m_direct3DDeviceIMContext->IASetVertexBuffers(StartSlot, 1, VertexBufferImp->Resource.GetAddressOf(), &Stride, &Offset);
}

void D3D11Device::SetRasterizerState(RasterizerStateImpl * State)
{
	D3D11RasterizerState* StateImp = static_cast<D3D11RasterizerState*>(State);
	m_direct3DDeviceIMContext->RSSetState(StateImp->Resource.Get());
}

void D3D11Device::SetBlendState(BlendStateImpl * State, const float BlendFactor[4], unsigned int SampleMask)
{
	D3D11BlendState* NewState = static_cast<D3D11BlendState*>(State);

	m_direct3DDeviceIMContext->OMSetBlendState(NewState->Resource.Get(), BlendFactor, SampleMask);
}

void D3D11Device::SetDepthStencilState(DepthStencilStateImpl * State, unsigned int StencilRef)
{
	D3D11DepthStencilState* NewState = static_cast<D3D11DepthStencilState*>(State);

	m_direct3DDeviceIMContext->OMSetDepthStencilState(NewState->Resource.Get(), StencilRef);
}

VertexDeclarationImpl * D3D11Device::CreateVertexDeclaration(vector<VertexElement>& Elements)
{
	D3D11VertexDeclarationKey Key(Elements);

	//VertexDeclarationCache.find(Key);

	VertexDeclarationImpl* VertexDeclaration = nullptr;// VertexDeclarationCache.find(Key)->second;
	if (VertexDeclaration == nullptr)
	{
		VertexDeclaration = new D3D11VertexDeclaration(Key.VertexElements);
		//VertexDeclarationCache.insert(pair<D3D11VertexDeclarationKey, VertexDeclarationImpl*>(Key, VertexDeclaration));
	}

	return VertexDeclaration;
}

BoundShaderStateImpl * D3D11Device::CreateBoundShaderState(VertexDeclarationImpl * VertexDeclaration, VertexShaderImpl * VertexShader, HullShaderImpl * HullShader, DomainShaderImpl * DomainShader, PixelShaderImpl * PixelShader, GeometryShaderImpl * GeometryShader)
{
	return new D3D11BoundShaderState(VertexDeclaration, VertexShader, PixelShader, HullShader, DomainShader, GeometryShader, m_direct3DDevice.Get());
}

void D3D11Device::SetBoundShaderState(BoundShaderStateImpl * BoundShaderState)
{
	D3D11BoundShaderState* BoundShaderStateImp = static_cast<D3D11BoundShaderState*>(BoundShaderState);

	m_direct3DDeviceIMContext->IASetInputLayout(BoundShaderStateImp->InputLayout.Get());

	m_direct3DDeviceIMContext->VSSetShader(BoundShaderStateImp->VertexShader.Get(), nullptr, 0);
	m_direct3DDeviceIMContext->PSSetShader(BoundShaderStateImp->PixelShader.Get(), nullptr, 0);

	m_direct3DDeviceIMContext->HSSetShader(BoundShaderStateImp->HullShader.Get(), nullptr, 0);
	m_direct3DDeviceIMContext->DSSetShader(BoundShaderStateImp->DomainShader.Get(), nullptr, 0);
	m_direct3DDeviceIMContext->GSSetShader(BoundShaderStateImp->GeometryShader.Get(), nullptr, 0);

	bUsingTessellation = BoundShaderStateImp->HullShader != nullptr && BoundShaderStateImp->DomainShader != nullptr;

	//ClearAllShaderResources();
}

Texture2DImpl * D3D11Device::CreateTexture2D(const void * pData, unsigned int SizeX, unsigned int SizeY, PIXEL_FORMAT Format, unsigned int NumMips, unsigned int NumSamples, unsigned int Flags)
{
	return CreateD3D11Texture2D<D3D11BaseTexture2D>(pData, SizeX, SizeY, 1, false, false, Format, NumMips, NumSamples, Flags);
}

void D3D11Device::clear(bool clearRenderTargets, bool ClearDepth, float Depth, bool ClearStencil, unsigned int Stencil)
{
	if (clearRenderTargets)
	{
		float ClearColor[4] = { 0.8f, 0.8f, 1.0f, 1.0f };

		for (size_t i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			ID3D11RenderTargetView *pRenderTargetView = CurrentRenderTargets[i].Get();
			if (pRenderTargetView != nullptr)
			{
				m_direct3DDeviceIMContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
			}
		}
	}


	ID3D11DepthStencilView *pDepthStencilView = CurrentDepthStencilTarget.Get();
	if (pDepthStencilView != nullptr)
	{
		unsigned int ClearFlags = 0;
		if (ClearDepth)
		{
			ClearFlags |= D3D11_CLEAR_DEPTH;
		}
		if (ClearStencil)
		{
			ClearFlags |= D3D11_CLEAR_STENCIL;
		}

		assert(Depth >= 0 && Depth <= 1);

		m_direct3DDeviceIMContext->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
	}
}

void D3D11Device::initD3DDevice()
{
	HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)DXGIFactory1.ReleaseAndGetAddressOf());

	if (!m_direct3DDevice)
	{
		//ComPtr<IDXGIAdapter> Adapter;

		//if (DXGIFactory1->EnumAdapters(1, Adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		//{

		//	DXGI_ADAPTER_DESC AdapterDesc;
		//	result = Adapter->GetDesc(&AdapterDesc);

		//	wstring AdapterName = AdapterDesc.Description;
		//	unsigned int VendorId = AdapterDesc.VendorId;//0x1002 - AMD, 0x8086 - Intel, 0x10DE - NVIDIA
		//	unsigned int DeviceId = AdapterDesc.DeviceId;
		//	unsigned int DeviceRevision = AdapterDesc.Revision;
		//}

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

		ThrowIfFailed(D3D11CreateDevice(nullptr, DriverType, nullptr, DeviceFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, m_direct3DDevice.ReleaseAndGetAddressOf(), &FeatureLevel, m_direct3DDeviceIMContext.ReleaseAndGetAddressOf()));
	}
}

RTVDesc D3D11Device::GetRenderTargetViewDesc(ID3D11RenderTargetView * RenderTargetView)
{
	D3D11_RENDER_TARGET_VIEW_DESC TargetDesc;
	RenderTargetView->GetDesc(&TargetDesc);

	ComPtr<ID3D11Resource> BaseResource;
	RenderTargetView->GetResource((ID3D11Resource**)BaseResource.ReleaseAndGetAddressOf());

	RTVDesc ret;
	memset(&ret, 0, sizeof(ret));

	switch (TargetDesc.ViewDimension)
	{
		case D3D11_RTV_DIMENSION_TEXTURE2D:
		case D3D11_RTV_DIMENSION_TEXTURE2DMS:
		case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
		case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
		{
			D3D11_TEXTURE2D_DESC Desc;
			((ID3D11Texture2D*)(BaseResource.Get()))->GetDesc(&Desc);
			ret.Width = Desc.Width;
			ret.Height = Desc.Height;
			ret.SampleDesc = Desc.SampleDesc;
		}
		break;
		case D3D11_RTV_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC Desc;
			((ID3D11Texture3D*)(BaseResource.Get()))->GetDesc(&Desc);
			ret.Width = Desc.Width;
			ret.Height = Desc.Height;
			ret.SampleDesc.Count = 1;
			ret.SampleDesc.Quality = 0;
		}
		break;
		default:
			break;
	}

	return ret;
}

D3D11_PRIMITIVE_TOPOLOGY D3D11Device::GetD3D11PrimitiveType(PRIMITIVE_TOPOLOGY PrimitiveType, bool bUsingTessellation)
{
	if (bUsingTessellation)
	{
		switch (PrimitiveType)
		{
		case PRIMITIVE_TOPOLOGY::CONTROL_POINT_PATCHLIST_1:
			return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
		case PRIMITIVE_TOPOLOGY::CONTROL_POINT_PATCHLIST_2:
			return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
		case PRIMITIVE_TOPOLOGY::TRIANGLELIST:
			return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
		default:
			break;
		}
	}

	switch (PrimitiveType)
	{
	case PRIMITIVE_TOPOLOGY::TRIANGLELIST:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PRIMITIVE_TOPOLOGY::TRIANGLESTRIP:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case PRIMITIVE_TOPOLOGY::LINELIST:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case PRIMITIVE_TOPOLOGY::POINTLIST:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	default:
		break;
	}

	return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

unsigned int D3D11Device::GetVertexCountForPrimitiveCount(unsigned int NumPrimitives, PRIMITIVE_TOPOLOGY PrimitiveType)
{
	unsigned int VertexCount = 0;

	switch (PrimitiveType)
	{
	case PRIMITIVE_TOPOLOGY::TRIANGLELIST:
		VertexCount = NumPrimitives * 3;
		break;
	case PRIMITIVE_TOPOLOGY::TRIANGLESTRIP:
		VertexCount = NumPrimitives + 2;
		break;
	case PRIMITIVE_TOPOLOGY::LINELIST:
		VertexCount = NumPrimitives * 2;
		break;
	case PRIMITIVE_TOPOLOGY::POINTLIST:
		VertexCount = NumPrimitives;
		break;
	case PRIMITIVE_TOPOLOGY::CONTROL_POINT_PATCHLIST_1:
	case PRIMITIVE_TOPOLOGY::CONTROL_POINT_PATCHLIST_2:
		VertexCount = ((unsigned int)PrimitiveType - 5 + 1) * NumPrimitives;
		break;
	default:
		break;
	}

	return VertexCount;
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

D3D11_COMPARISON_FUNC D3D11Device::ConvertSamplerCompareFunction(SAMPLER_COMPARE_FUNCTION SamplerComparisonFunction)
{
	switch (SamplerComparisonFunction)
	{
	case SAMPLER_COMPARE_FUNCTION::LESS:
		return D3D11_COMPARISON_LESS;
	case SAMPLER_COMPARE_FUNCTION::NEVER:
	default:
		return D3D11_COMPARISON_NEVER;
	}
}

D3D11_CULL_MODE D3D11Device::ConvertCullMode(RASTERIZER_CULL_MODE CullMode)
{
	switch (CullMode)
	{
	case RASTERIZER_CULL_MODE::CW:
		return D3D11_CULL_BACK;
	case RASTERIZER_CULL_MODE::CCW:
		return D3D11_CULL_FRONT;
	case RASTERIZER_CULL_MODE::NONE:
	default:
		return D3D11_CULL_NONE;
	}
}

D3D11_FILL_MODE D3D11Device::ConvertFillMode(RASTERIZER_FILL_MODE FillMode)
{
	switch (FillMode)
	{
	case RASTERIZER_FILL_MODE::WIREFRAME:
		return D3D11_FILL_WIREFRAME;
	case RASTERIZER_FILL_MODE::SOLID:
	default:
		return D3D11_FILL_SOLID;
	}
}

D3D11_COMPARISON_FUNC D3D11Device::ConvertCompareFunction(COMPARE_FUNCTION CompareFunction)
{
	switch (CompareFunction)
	{
	case COMPARE_FUNCTION::LESS:
		return D3D11_COMPARISON_LESS;
	case COMPARE_FUNCTION::LESSEQUAL:
		return D3D11_COMPARISON_LESS_EQUAL;
	case COMPARE_FUNCTION::GREATER:
		return D3D11_COMPARISON_GREATER;
	case COMPARE_FUNCTION::GREATEREQUAL:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case COMPARE_FUNCTION::EQUAL:
		return D3D11_COMPARISON_EQUAL;
	case COMPARE_FUNCTION::NOTEQUAL:
		return D3D11_COMPARISON_NOT_EQUAL;
	case COMPARE_FUNCTION::NEVER:
		return D3D11_COMPARISON_NEVER;
	case COMPARE_FUNCTION::ALWAYS:
	default:
		return D3D11_COMPARISON_ALWAYS;
	}
}

D3D11_STENCIL_OP D3D11Device::ConvertStencilOp(STENCIL_OP StencilOp)
{
	switch (StencilOp)
	{
	case STENCIL_OP::ZERO:
		return D3D11_STENCIL_OP_ZERO;
	case STENCIL_OP::REPLACE:
		return D3D11_STENCIL_OP_REPLACE;
	case STENCIL_OP::SATURATEDINCREMENT:
		return D3D11_STENCIL_OP_INCR_SAT;
	case STENCIL_OP::SATURATEDDECREMENT:
		return D3D11_STENCIL_OP_DECR_SAT;
	case STENCIL_OP::INVERT:
		return D3D11_STENCIL_OP_INVERT;
	case STENCIL_OP::INCREMENT:
		return D3D11_STENCIL_OP_INCR;
	case STENCIL_OP::DECREMENT:
		return D3D11_STENCIL_OP_DECR;
	case STENCIL_OP::KEEP:
	default:
		return D3D11_STENCIL_OP_KEEP;
	}
}

unsigned int D3D11Device::ComputeAnisotropyRT(int InitializerMaxAnisotropy)
{
	int CVarValue = 0;

	return SkyMath::clamp(InitializerMaxAnisotropy > 0 ? InitializerMaxAnisotropy : CVarValue, 1, 16);
}

//void D3D11Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC * TextureDesc, const D3D11_SUBRESOURCE_DATA * SubResourceData, ID3D11Texture2D ** OutTexture2D)
//{
//	m_direct3DDevice->CreateTexture2D(TextureDesc, SubResourceData, OutTexture2D);
//}

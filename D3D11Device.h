#pragma once
#include "D3D11Resources.h"
#include "RenderTargetViewImpl.h"
#include "DepthRenderTargetViewImpl.h"
#include <map>
#include "common.h"
#include "utility.h"

using namespace std;

struct RenderTargetsInfo
{
	RenderTargetViewImpl RenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	int RenderTargetsCount;

	DepthRenderTargetViewImpl *DepthStencilRenderTarget;
};

struct RTVDesc
{
	unsigned int Width;
	unsigned int Height;
	DXGI_SAMPLE_DESC SampleDesc;
};

class SKYENGINEDLL D3D11Device
{
public:
	D3D11Device();
	virtual ~D3D11Device();

	void init();
	void SetRenderTargets(const RenderTargetsInfo& renderTargetsInfo);
	void SetViewport(unsigned int MinX, unsigned int MinY, float MinZ, unsigned int MaxX, unsigned int MaxY, float MaxZ);
	void DrawPrimitive(PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int BaseVertexIndex, unsigned int NumPrimitives, unsigned int NumInstances);
	void DrawIndexedPrimitive(IndexBufferImpl* IndexBuffer, PRIMITIVE_TOPOLOGY PrimitiveType, int BaseVertexIndex, unsigned int FirstInstance, unsigned int NumVertices, unsigned int StartIndex, unsigned int NumPrimitives, unsigned int NumInstances);
	SamplerStateImpl* CreateSamplerState(const SamplerStateDesc& desc);
	RasterizerStateImpl* CreateRasterizerState(const RasterizerStateDesc& desc);
	DepthStencilStateImpl* CreateDepthStencilState(const DepthStencilStateDesc& desc);
	BlendStateImpl* CreateBlendState(const BlendStateDesc& desc);
	VertexShaderImpl* CreateVertexShader(const void* pByteCode, size_t ByteCodeLength);
	HullShaderImpl* CreateHullShader(const void* pByteCode, size_t ByteCodeLength);
	DomainShaderImpl* CreateDomainShader(const void* pByteCode, size_t ByteCodeLength);
	PixelShaderImpl* CreatePixelShader(const void* pByteCode, size_t ByteCodeLength);
	GeometryShaderImpl* CreateGeometryShader(const void* pByteCode, size_t ByteCodeLength);
	ComputeShaderImpl* CreateComputeShader(const void* pByteCode, size_t ByteCodeLength);
	IndexBufferImpl* CreateIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage);
	void* LockIndexBuffer(IndexBufferImpl* IndexBuffer, unsigned int Offset, unsigned int Size, RESOURCE_LOCK_MODE LockMode);
	void UnlockIndexBuffer(IndexBufferImpl* IndexBuffer);
	VertexBufferImpl* CreateVertexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage);
	void* LockVertexBuffer(VertexBufferImpl* VertexBuffer, unsigned int Offset, unsigned int Size, RESOURCE_LOCK_MODE LockMode);
	void UnlockVertexBuffer(VertexBufferImpl* VertexBuffer);
	void CopyVertexBuffer(VertexBufferImpl* SourceBuffer, VertexBufferImpl* DestBuffer);
	void SetVertexBuffer(VertexBufferImpl* VertexBuffer, unsigned int StartSlot, unsigned int Stride, unsigned int Offset);
	void SetRasterizerState(RasterizerStateImpl* NewState);
	void SetBlendState(BlendStateImpl* State, const float BlendFactor[4], unsigned int SampleMask = 0xffffffff);
	void SetDepthStencilState(DepthStencilStateImpl* State, unsigned int StencilRef);
	VertexDeclarationImpl* CreateVertexDeclaration(vector<VertexElement>& Elements);
	BoundShaderStateImpl* CreateBoundShaderState(VertexDeclarationImpl* VertexDeclaration, VertexShaderImpl* VertexShader, HullShaderImpl* HullShaderRHI, DomainShaderImpl* DomainShader,PixelShaderImpl* PixelShader,GeometryShaderImpl* GeometryShader);
	void SetBoundShaderState(BoundShaderStateImpl* BoundShaderState);
	Texture2DImpl* CreateTexture2D(const void *pData, unsigned int SizeX, unsigned int SizeY, PIXEL_FORMAT Format, unsigned int NumMips, unsigned int NumSamples, unsigned int Flags);
	template<typename BaseResourceType>
	TD3D11Texture2D<BaseResourceType>* CreateD3D11Texture2D(const void *pData, unsigned int SizeX, unsigned int SizeY, unsigned int SizeZ, bool bTextureArray, bool bCubeTexture, PIXEL_FORMAT Format, unsigned int NumMips, unsigned int NumSamples, unsigned int Flags);
	void clear(bool clearRenderTargets, bool ClearDepth, float Depth, bool ClearStencil, unsigned int Stencil = 0);
	ID3D11Device* GetDevice() const
	{
		return m_direct3DDevice.Get();
	}
	ID3D11DeviceContext* GetDeviceContext() const
	{
		return m_direct3DDeviceIMContext.Get();
	}
	IDXGIFactory1* GetFactory() const
	{
		return DXGIFactory1.Get();
	}
private:
	void initD3DDevice();
	RTVDesc GetRenderTargetViewDesc(ID3D11RenderTargetView* RenderTargetView);
	D3D11_PRIMITIVE_TOPOLOGY GetD3D11PrimitiveType(PRIMITIVE_TOPOLOGY PrimitiveType, bool bUsingTessellation);
	unsigned int GetVertexCountForPrimitiveCount(unsigned int NumPrimitives, PRIMITIVE_TOPOLOGY PrimitiveType);
	D3D11_TEXTURE_ADDRESS_MODE ConvertAddressMode(SAMPLER_ADDRESS_MODE AddressMode);
	D3D11_COMPARISON_FUNC ConvertSamplerCompareFunction(SAMPLER_COMPARE_FUNCTION SamplerComparisonFunction);
	D3D11_CULL_MODE ConvertCullMode(RASTERIZER_CULL_MODE CullMode);
	D3D11_FILL_MODE ConvertFillMode(RASTERIZER_FILL_MODE FillMode);
	D3D11_COMPARISON_FUNC ConvertCompareFunction(COMPARE_FUNCTION CompareFunction);
	D3D11_STENCIL_OP ConvertStencilOp(STENCIL_OP StencilOp);
	unsigned int ComputeAnisotropyRT(int InitializerMaxAnisotropy);
	//void CreateTexture2D(const D3D11_TEXTURE2D_DESC* TextureDesc, const D3D11_SUBRESOURCE_DATA* SubResourceData, ID3D11Texture2D** OutTexture2D);

	ComPtr<ID3D11DeviceContext> m_direct3DDeviceIMContext;
	ComPtr<ID3D11Device> m_direct3DDevice;
	ComPtr<IDXGIFactory1> DXGIFactory1;
	D3D_FEATURE_LEVEL FeatureLevel;

	ComPtr<ID3D11RenderTargetView> CurrentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ComPtr<ID3D11DepthStencilView> CurrentDepthStencilTarget;
	shared_ptr<D3D11TextureBase> CurrentDepthTexture;
	D3D11_PRIMITIVE_TOPOLOGY CurrentPrimitiveTopology;

	bool bUsingTessellation;

	map<ID3D11SamplerState*, D3D11SamplerState*> SamplerStateCache;
	map<D3D11VertexDeclarationKey, VertexDeclarationImpl*> VertexDeclarationCache;
};

template<typename BaseResourceType>
inline TD3D11Texture2D<BaseResourceType>* D3D11Device::CreateD3D11Texture2D(const void * pData, unsigned int SizeX, unsigned int SizeY, unsigned int SizeZ, bool bTextureArray, bool bCubeTexture, PIXEL_FORMAT Format, unsigned int NumMips, unsigned int NumSamples, unsigned int Flags)
{
	assert(SizeX > 0 && SizeY > 0 && NumMips > 0);

	if (bCubeTexture)
	{
		assert(SizeX <= 2048);
		assert(SizeX == SizeY);
	}
	else
	{
		assert(SizeX <= 2048);
		assert(SizeY <= 2048);
	}

	if (bTextureArray)
	{
		assert(SizeZ <= 256);
	}

	bool bPooledTexture = true;

	const DXGI_FORMAT PlatformResourceFormat = (DXGI_FORMAT)Format;
	const DXGI_FORMAT PlatformShaderResourceFormat = (DXGI_FORMAT)Format;
	const DXGI_FORMAT PlatformRenderTargetFormat = (DXGI_FORMAT)Format;

	D3D11_DSV_DIMENSION DepthStencilViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	D3D11_RTV_DIMENSION RenderTargetViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	D3D11_SRV_DIMENSION ShaderResourceViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	unsigned int CPUAccessFlags = 0;
	D3D11_USAGE TextureUsage = D3D11_USAGE_DEFAULT;
	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bool bCreateShaderResource = true;

	unsigned int ActualMSAACount = NumSamples;

	unsigned int ActualMSAAQuality = 0;

	if (ActualMSAACount > 1)
	{
		DepthStencilViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		RenderTargetViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		ShaderResourceViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		bPooledTexture = false;
	}

	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	TextureDesc.Width = SizeX;
	TextureDesc.Height = SizeY;
	TextureDesc.MipLevels = NumMips;
	TextureDesc.ArraySize = SizeZ;
	TextureDesc.Format = PlatformResourceFormat;
	TextureDesc.SampleDesc.Count = ActualMSAACount;
	TextureDesc.SampleDesc.Quality = ActualMSAAQuality;
	TextureDesc.Usage = TextureUsage;
	TextureDesc.BindFlags = BindFlags;
	TextureDesc.CPUAccessFlags = CPUAccessFlags;
	TextureDesc.MiscFlags = bCubeTexture ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

	bool bCreateRTV = false;
	bool bCreateDSV = false;
	bool bCreatedRTVPerSlice = false;

	if (Format == PIXEL_FORMAT::PF_DepthStencil || Format == PIXEL_FORMAT::PF_ShadowDepth || Format == PIXEL_FORMAT::PF_D24)
	{
		TextureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		bCreateDSV = true;
	}
	else
	{
		TextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		bCreateRTV = true;
	}

	if (bCreateDSV || bCreateRTV || bCubeTexture || bTextureArray)
	{
		bPooledTexture = false;
	}

	ComPtr<ID3D11Texture2D> TextureResource;
	ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
	vector<ComPtr<ID3D11RenderTargetView> > RenderTargetViews;
	ComPtr<ID3D11DepthStencilView> DepthStencilViews[4];

	vector<D3D11_SUBRESOURCE_DATA> SubResourceData;

	if (pData)
	{
		SubResourceData.reserve(NumMips * SizeZ);
		SubResourceData.resize(NumMips * SizeZ);

		unsigned int SliceOffset = 0;
		for (unsigned int ArraySliceIndex = 0; ArraySliceIndex < SizeZ; ++ArraySliceIndex)
		{
			unsigned int MipOffset = 0;
			for (unsigned int MipIndex = 0; MipIndex < NumMips; ++MipIndex)
			{
				unsigned int SubResourceIndex = ArraySliceIndex * NumMips + MipIndex;

				SubResourceData[SubResourceIndex].pSysMem = pData;
				//SubResourceData[SubResourceIndex].SysMemPitch = NumBlocksX * GPixelFormats[Format].BlockBytes;
				//SubResourceData[SubResourceIndex].SysMemSlicePitch = NumBlocksX * NumBlocksY * SubResourceData[MipIndex].SysMemPitch;
			}
			SliceOffset += MipOffset;
		}
	}

	m_direct3DDevice->CreateTexture2D(&TextureDesc, pData != nullptr ? (const D3D11_SUBRESOURCE_DATA*)SubResourceData.data() : nullptr, TextureResource.ReleaseAndGetAddressOf());

	if (bCreateRTV)
	{
		for (unsigned int MipIndex = 0; MipIndex < NumMips; MipIndex++)
		{
			D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
			ZeroMemory(&RTVDesc, sizeof(RTVDesc));
			RTVDesc.Format = PlatformRenderTargetFormat;
			if (bTextureArray || bCubeTexture)
			{
				RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				RTVDesc.Texture2DArray.FirstArraySlice = 0;
				RTVDesc.Texture2DArray.ArraySize = TextureDesc.ArraySize;
				RTVDesc.Texture2DArray.MipSlice = MipIndex;
			}
			else
			{
				RTVDesc.ViewDimension = RenderTargetViewDimension;
				RTVDesc.Texture2D.MipSlice = MipIndex;
			}

			ComPtr<ID3D11RenderTargetView> RenderTargetView;
			m_direct3DDevice->CreateRenderTargetView(TextureResource.Get(), &RTVDesc, RenderTargetView.ReleaseAndGetAddressOf());
			RenderTargetViews.push_back(RenderTargetView);
		}
	}

	if (bCreateDSV)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		ZeroMemory(&DSVDesc, sizeof(DSVDesc));
		DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;// FindDepthStencilDXGIFormat(PlatformResourceFormat);
		if (bTextureArray || bCubeTexture)
		{
			DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			DSVDesc.Texture2DArray.FirstArraySlice = 0;
			DSVDesc.Texture2DArray.ArraySize = TextureDesc.ArraySize;
			DSVDesc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			DSVDesc.ViewDimension = DepthStencilViewDimension;
			DSVDesc.Texture2D.MipSlice = 0;
		}

		for (unsigned int AccessType = 0; AccessType < 4; ++AccessType)
		{

			DSVDesc.Flags = (AccessType & 33) ? D3D11_DSV_READ_ONLY_DEPTH : 0;
			if (DSVDesc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
			{
				DSVDesc.Flags |= (AccessType & 18) ? D3D11_DSV_READ_ONLY_STENCIL : 0;
			}
			m_direct3DDevice->CreateDepthStencilView(TextureResource.Get(), &DSVDesc, DepthStencilViews[AccessType].ReleaseAndGetAddressOf());
		}
	}

	if (bCreateShaderResource)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = PlatformShaderResourceFormat;

		if (bCubeTexture && bTextureArray)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			SRVDesc.TextureCubeArray.MostDetailedMip = 0;
			SRVDesc.TextureCubeArray.MipLevels = NumMips;
			SRVDesc.TextureCubeArray.First2DArrayFace = 0;
			SRVDesc.TextureCubeArray.NumCubes = SizeZ / 6;
		}
		else if (bCubeTexture)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SRVDesc.TextureCube.MostDetailedMip = 0;
			SRVDesc.TextureCube.MipLevels = NumMips;
		}
		else if (bTextureArray)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			SRVDesc.Texture2DArray.MostDetailedMip = 0;
			SRVDesc.Texture2DArray.MipLevels = NumMips;
			SRVDesc.Texture2DArray.FirstArraySlice = 0;
			SRVDesc.Texture2DArray.ArraySize = TextureDesc.ArraySize;
		}
		else
		{
			SRVDesc.ViewDimension = ShaderResourceViewDimension;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = NumMips;
		}
		m_direct3DDevice->CreateShaderResourceView(TextureResource.Get(), &SRVDesc, ShaderResourceView.ReleaseAndGetAddressOf());
	}

	TD3D11Texture2D<BaseResourceType>* Texture2D = new TD3D11Texture2D<BaseResourceType>(
		TextureResource.Get(),
		ShaderResourceView.Get(),
		bCreatedRTVPerSlice,
		TextureDesc.ArraySize,
		RenderTargetViews,
		DepthStencilViews,
		SizeX,
		SizeY,
		SizeZ,
		NumMips,
		ActualMSAACount,
		Format,
		bCubeTexture,
		Flags,
		bPooledTexture);

	return Texture2D;
}

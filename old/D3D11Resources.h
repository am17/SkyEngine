#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "ResourcesImpl.h"
#include <assert.h> 
#include <vector>
#include <memory>
#include <string>

using Microsoft::WRL::ComPtr;
using namespace std;

class D3D11SamplerState : public SamplerStateImpl
{
public:
	ComPtr<ID3D11SamplerState> Resource;
};

class D3D11RasterizerState : public RasterizerStateImpl
{
public:
	ComPtr<ID3D11RasterizerState> Resource;
};

class D3D11DepthStencilState : public DepthStencilStateImpl
{
public:
	ComPtr<ID3D11DepthStencilState> Resource;
};

class D3D11BlendState : public BlendStateImpl
{
public:
	ComPtr<ID3D11BlendState> Resource;
};

class D3D11VertexShader : public VertexShaderImpl
{
public:
	ComPtr<ID3D11VertexShader> Resource;
	void* Code;
	size_t ByteCodeLength;
};

class D3D11HullShader : public HullShaderImpl
{
public:
	ComPtr<ID3D11HullShader> Resource;
};

class D3D11DomainShader : public DomainShaderImpl
{
public:
	ComPtr<ID3D11DomainShader> Resource;
};

class D3D11PixelShader : public PixelShaderImpl
{
public:
	ComPtr<ID3D11PixelShader> Resource;
};

class D3D11GeometryShader : public GeometryShaderImpl
{
public:
	ComPtr<ID3D11GeometryShader> Resource;
};

class D3D11ComputeShader : public ComputeShaderImpl
{
public:
	ComPtr<ID3D11ComputeShader> Resource;
};

class D3D11VertexDeclaration : public VertexDeclarationImpl
{
public:
	D3D11VertexDeclaration(vector<D3D11_INPUT_ELEMENT_DESC>& InElements)
		:VertexElements(InElements)
	{}
	vector<D3D11_INPUT_ELEMENT_DESC> VertexElements;
};

class D3D11BoundShaderState : public BoundShaderStateImpl
{
public:
	D3D11BoundShaderState(VertexDeclarationImpl* aVertexDeclaration, VertexShaderImpl* aVertexShader, PixelShaderImpl* aPixelShader, HullShaderImpl* aHullShader, DomainShaderImpl* aDomainShader, GeometryShaderImpl* aGeometryShader, ID3D11Device* Direct3DDevice)
	{
		D3D11VertexDeclaration* InVertexDeclaration = static_cast<D3D11VertexDeclaration*>(aVertexDeclaration);
		D3D11VertexShader* InVertexShader = static_cast<D3D11VertexShader*>(aVertexShader);
		D3D11PixelShader* InPixelShader = static_cast<D3D11PixelShader*>(aPixelShader);
		D3D11HullShader* InHullShader = static_cast<D3D11HullShader*>(aHullShader);
		D3D11DomainShader* InDomainShader = static_cast<D3D11DomainShader*>(aDomainShader);
		D3D11GeometryShader* InGeometryShader = static_cast<D3D11GeometryShader*>(aGeometryShader);

		D3D11_INPUT_ELEMENT_DESC NullInputElement;
		ZeroMemory(&NullInputElement, sizeof(D3D11_INPUT_ELEMENT_DESC));

		HRESULT hr = Direct3DDevice->CreateInputLayout(InVertexDeclaration ? InVertexDeclaration->VertexElements.data() : &NullInputElement, InVertexDeclaration ? InVertexDeclaration->VertexElements.size() : 0, InVertexShader->Code, InVertexShader->ByteCodeLength, InputLayout.ReleaseAndGetAddressOf());

		VertexShader = InVertexShader->Resource;
		PixelShader = InPixelShader ? InPixelShader->Resource : nullptr;
		HullShader = InHullShader ? InHullShader->Resource : nullptr;
		DomainShader = InDomainShader ? InDomainShader->Resource : nullptr;
		GeometryShader = InGeometryShader ? InGeometryShader->Resource : nullptr;
	}
	virtual ~D3D11BoundShaderState() {}

	//FORCEINLINE D3D11VertexShader*   GetVertexShader() const { return VertexShader.Get(); }
	//FORCEINLINE D3D11PixelShader*    GetPixelShader() const { return (FD3D11PixelShader*)CacheLink.GetPixelShader(); }
	//FORCEINLINE D3D11HullShader*     GetHullShader() const { return (FD3D11HullShader*)CacheLink.GetHullShader(); }
	//FORCEINLINE D3D11DomainShader*   GetDomainShader() const { return (FD3D11DomainShader*)CacheLink.GetDomainShader(); }
	//FORCEINLINE D3D11GeometryShader* GetGeometryShader() const { return (FD3D11GeometryShader*)CacheLink.GetGeometryShader(); }
	ComPtr<ID3D11InputLayout> InputLayout;
	ComPtr<ID3D11VertexShader> VertexShader;
	ComPtr<ID3D11PixelShader> PixelShader;
	ComPtr<ID3D11HullShader> HullShader;
	ComPtr<ID3D11DomainShader> DomainShader;
	ComPtr<ID3D11GeometryShader> GeometryShader;
};

class D3D11IndexBuffer : public IndexBufferImpl
{
public:
	ComPtr<ID3D11Buffer> Resource;

	D3D11IndexBuffer(ID3D11Buffer* resource, unsigned int stride, unsigned int size, BUFFER_USAGE_FLAGS usage)
		: IndexBufferImpl(stride, size, usage), Resource(resource)
	{}

	virtual ~D3D11IndexBuffer(){ }
};

class D3D11VertexBuffer : public VertexBufferImpl
{
public:
	ComPtr<ID3D11Buffer> Resource;

	D3D11VertexBuffer(ID3D11Buffer* resource, unsigned int size, BUFFER_USAGE_FLAGS usage)
		: VertexBufferImpl(size, usage), Resource(resource)
	{}

	virtual ~D3D11VertexBuffer(){ }
};

class D3D11BaseTexture2D : public Texture2DImpl
{
public:
	D3D11BaseTexture2D(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int numMips, unsigned int numSamples, PIXEL_FORMAT format, unsigned int flags)
		: Texture2DImpl(sizeX, sizeY, numMips, numSamples, format, flags)
	{}
	unsigned int getSizeZ() const { return 0; }
};

class D3D11BaseTexture2DArray : public Texture2DArrayImpl
{
public:
	D3D11BaseTexture2DArray(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int numMips, unsigned int numSamples, PIXEL_FORMAT format, unsigned int flags)
		: Texture2DArrayImpl(sizeX, sizeY, sizeZ, numMips, format, flags)
	{
		assert(numSamples == 1);
	}
};

class D3D11TextureBase
{
public:
	D3D11TextureBase(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView, unsigned int RTVArraySize, bool bInCreatedRTVsPerSlice, const vector<ComPtr<ID3D11RenderTargetView>>& renderTargetViews, ComPtr<ID3D11DepthStencilView>* depthStencilViews):
		m_resource(resource), m_shaderResourceView(shaderResourceView), m_renderTargetViews(renderTargetViews), bCreatedRTVsPerSlice(bInCreatedRTVsPerSlice), RTVArraySize(RTVArraySize), m_numDepthStencilViews(0)	
	{
		if (depthStencilViews != nullptr)
		{
			for (int index = 0; index < 4; index++)
			{
				m_depthStencilViews[index] = depthStencilViews[index];

				if (m_depthStencilViews[index] != nullptr)
				{
					m_numDepthStencilViews++;
				}
			}
		}
	}
	virtual ~D3D11TextureBase() {}

	ID3D11Resource* getResource() const { return m_resource.Get(); }

	ID3D11RenderTargetView* getRenderTargetView(int MipIndex, int ArraySliceIndex) const
	{
		int ArrayIndex = MipIndex;

		if (bCreatedRTVsPerSlice)
		{
			assert(ArraySliceIndex >= 0);
			ArrayIndex = MipIndex * RTVArraySize + ArraySliceIndex;
		}
		else
		{
			assert(ArraySliceIndex == -1 || ArraySliceIndex == 0);
		}

		if ((unsigned int)ArrayIndex < (unsigned int)m_renderTargetViews.size())
		{
			return m_renderTargetViews[ArrayIndex].Get();
		}
		return 0;
	}
	ID3D11DepthStencilView* getDepthStencilView(int index) const
	{
		return m_depthStencilViews[index].Get();
	}
protected:
	ComPtr<ID3D11Resource> m_resource;
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	vector<ComPtr<ID3D11RenderTargetView>> m_renderTargetViews;
	bool bCreatedRTVsPerSlice;
	int RTVArraySize;
	ComPtr<ID3D11DepthStencilView> m_depthStencilViews[4];
	unsigned int m_numDepthStencilViews;
};

class D3D11Texture3D : public Texture3DImpl, public D3D11TextureBase
{
public:
	D3D11Texture3D(ID3D11Texture3D* resource, ID3D11ShaderResourceView* shaderResourceView,
		const vector<ComPtr<ID3D11RenderTargetView>>& renderTargetViews,
		unsigned int sizeX,
		unsigned int sizeY,
		unsigned int sizeZ,
		unsigned int numMips,
		PIXEL_FORMAT format,
		unsigned int flags)
		:Texture3DImpl(sizeX, sizeY, sizeZ, numMips, format, flags),
		D3D11TextureBase(resource, shaderResourceView, 1, false, renderTargetViews, nullptr)
	{}
	virtual ~D3D11Texture3D() {}
	ID3D11Texture3D* getResource() const { return (ID3D11Texture3D*)D3D11TextureBase::getResource(); }
};

class D3D11BaseTextureCube : public TextureCubeImpl
{
public:
	D3D11BaseTextureCube(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int numMips, unsigned int numSamples, PIXEL_FORMAT format, unsigned int flags)
		: TextureCubeImpl(sizeX, numMips, format, flags)
	{
		assert(numSamples == 1);
	}
	unsigned int getSizeX() const { return getSize(); }
	unsigned int getSizeY() const { return getSize(); }
	unsigned int getSizeZ() const { return 0; }
};

template<typename BaseResourceType>
class TD3D11Texture2D : public BaseResourceType, public D3D11TextureBase
{
public:
	TD3D11Texture2D(ID3D11Texture2D* InResource,
		ID3D11ShaderResourceView* InShaderResourceView,
		bool bInCreatedRTVsPerSlice,
		int InRTVArraySize,
		const vector<ComPtr<ID3D11RenderTargetView>>& InRenderTargetViews,
		ComPtr<ID3D11DepthStencilView>* InDepthStencilViews,
		unsigned int InSizeX,
		unsigned int InSizeY,
		unsigned int InSizeZ,
		unsigned int InNumMips,
		unsigned int InNumSamples,
		PIXEL_FORMAT InFormat,
		bool bInCubemap,
		unsigned int InFlags,
		bool bInPooled)
		:BaseResourceType(InSizeX,InSizeY,InSizeZ,InNumMips,InNumSamples,InFormat,InFlags), 
		D3D11TextureBase(InResource, InShaderResourceView, InRTVArraySize, bInCreatedRTVsPerSlice, InRenderTargetViews, InDepthStencilViews), 
		Flags(InFlags), bCubemap(bInCubemap), bPooled(bInPooled)
	{
	}
	virtual ~TD3D11Texture2D() {}
	unsigned int Flags;

	void* lock(unsigned int MipIndex, unsigned int ArrayIndex, RESOURCE_LOCK_MODE LockMode, unsigned int& DestStride)
	{

	}

	void unlock(unsigned int MipIndex, unsigned int ArrayIndex)
	{

	}

	ID3D11Texture2D* GetResource() const { return (ID3D11Texture2D*)D3D11TextureBase::getResource(); }
	bool IsCubemap() const { return bCubemap; }
private:
	const unsigned int bCubemap : 1;
	const unsigned int bPooled : 1;
};

typedef TD3D11Texture2D<TextureImpl>				D3D11Texture;
typedef TD3D11Texture2D<D3D11BaseTexture2D>			D3D11Texture2D;
typedef TD3D11Texture2D<D3D11BaseTexture2DArray>	D3D11Texture2DArray;
typedef TD3D11Texture2D<D3D11BaseTextureCube>		D3D11TextureCube;

struct D3D11VertexDeclarationKey
{
	vector<D3D11_INPUT_ELEMENT_DESC> VertexElements;
	unsigned int Hash;

	D3D11VertexDeclarationKey(vector<VertexElement>& Elements)
	{
		for (vector<VertexElement>::iterator it = Elements.begin(); it != Elements.end(); ++it) {
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

			D3DElement.SemanticName = "ATTRIBUTE";
			D3DElement.SemanticIndex = Element.AttributeIndex;
			D3DElement.InputSlotClass = Element.UseInstanceIndex ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
			D3DElement.InstanceDataStepRate = Element.UseInstanceIndex ? 1 : 0;

			VertexElements.push_back(D3DElement);
		}

		//Hash = FCrc::MemCrc_DEPRECATED(VertexElements.data(), VertexElements.size() * sizeof(D3D11_INPUT_ELEMENT_DESC));
	}

	unsigned int GetTypeHash(const D3D11VertexDeclarationKey& Key)
	{
		return Key.Hash;
	}

	FORCEINLINE bool operator()(const D3D11_INPUT_ELEMENT_DESC& A, const D3D11_INPUT_ELEMENT_DESC &B) const
	{
		return ((int)A.AlignedByteOffset + A.InputSlot * 0xffff) < ((int)B.AlignedByteOffset + B.InputSlot * 0xffff);
	}

	/*bool operator==(const D3D11VertexDeclarationKey& A, const D3D11VertexDeclarationKey& B)
	{
		return A.VertexElements == B.VertexElements;
	}*/
};
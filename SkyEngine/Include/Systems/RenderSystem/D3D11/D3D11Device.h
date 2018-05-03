#pragma once
#include "../IDeviceImpl.h"
#include "d3d11.h"
#include <memory>
#include <wrl/client.h>
#include "common.h"

class SKYENGINEDLL D3D11Device : public IDeviceImpl
{
public:
	D3D11Device();
	virtual ~D3D11Device();
	bool Init() override;
	void *CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality) override;
	ID3D11Device *GetDevice() const { return mDirect3DDevice.Get();  }
	IDXGIFactory1 *GetFactory() const { return mDXGIFactory.Get(); }
	ID3D11DeviceContext *GetContext() const { return mD3DDeviceIMContext.Get(); }
	VertexShader* CreateVertexShader(const void* pByteCode, size_t ByteCodeLength) override;
	HullShader* CreateHullShader(const void* pByteCode, size_t ByteCodeLength) override;
	DomainShader* CreateDomainShader(const void* pByteCode, size_t ByteCodeLength) override;
	PixelShader* CreatePixelShader(const void* pByteCode, size_t ByteCodeLength) override;
	GeometryShader* CreateGeometryShader(const void* pByteCode, size_t ByteCodeLength) override;
	ComputeShader* CreateComputeShader(const void* pByteCode, size_t ByteCodeLength) override;
	SamplerState* CreateSamplerState(const SamplerStateDesc& Initializer) override;
	BlendState* CreateBlendState() override;
	ConstantBuffer* CreateConstantBuffer(unsigned int BufferSize) override;
	VertexDeclaration* CreateVertexDeclaration(vector<VertexElement>& Elements) override;
	RasterizerState* CreateRasterizerState(const RasterizerDesc& Initializer) override;
	DepthStencilState* CreateDepthStencilState(const DepthStencilDesc& Initializer) override;
	VertexBuffer* CreateVertexBuffer(const void * pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage) override;
	IndexBuffer* CreateIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage) override;
	void SetVertexBuffer(VertexBuffer *vertexBuffer, unsigned int StartSlot, unsigned int Stride, unsigned int Offset) override;
	void SetIndexBuffer(IndexBuffer *indexBuffer) override;
	void Draw(unsigned int vertexCount) override;
	void DrawIndexedPrimitive(unsigned int NumPrimitives, unsigned int StartIndex, int BaseVertexIndex) override;
	void SetViewport(ViewPort *pViewPort) override;
	void SetBlendState(COMMON_BLEND_STATES state) override;
	void SetDepthStencilState(COMMON_DEPTH_STENCIL_STATES state) override;
	void SetRasterizerState(COMMON_RASTERIZER_STATES state) override;
	void SetSamplerState(COMMON_SAMPLER_STATES state) override;
private:
	D3D11_TEXTURE_ADDRESS_MODE ConvertAddressMode(SAMPLER_ADDRESS_MODE AddressMode);
	D3D11_COMPARISON_FUNC ConvertCompareFunction(COMPARISON_FUNCTION ComparisonFunction);
	void CreateCommonStates();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mD3DDeviceIMContext;
	Microsoft::WRL::ComPtr<ID3D11Device> mDirect3DDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory1> mDXGIFactory;
	D3D_FEATURE_LEVEL FeatureLevel;

	//common states
	ID3D11BlendState* Opaque;
	ID3D11BlendState* AlphaBlend;
	ID3D11BlendState* Additive;
	ID3D11BlendState* NonPremultiplied;

	ID3D11DepthStencilState* DepthDefault;
	ID3D11DepthStencilState* DepthRead;
	ID3D11DepthStencilState* DepthNone;

	ID3D11RasterizerState* CullNone;
	ID3D11RasterizerState* CullClockwise;
	ID3D11RasterizerState* CullCounterClockwise;
	ID3D11RasterizerState* Wireframe;

	ID3D11SamplerState* PointWrap;
	ID3D11SamplerState* PointClamp;
	ID3D11SamplerState* LinearWrap;
	ID3D11SamplerState* LinearClamp;
	ID3D11SamplerState* AnisotropicWrap;
	ID3D11SamplerState* AnisotropicClamp;
};


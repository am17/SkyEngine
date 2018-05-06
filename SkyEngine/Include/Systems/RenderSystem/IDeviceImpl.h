#pragma once
#include <Windows.h>
#include "Shader.h"
#include "SamplerStateDesc.h"
#include "ConstantBuffer.h"
#include "RasterizerDesc.h"
#include "DepthStencilDesc.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include <vector>

using namespace std;

class IDeviceImpl
{
public:
	virtual ~IDeviceImpl() {}
	virtual bool Init() = 0;
	virtual Texture2D *CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount = 1, unsigned int multiSampleQuality = 0) = 0;
	virtual VertexShader* CreateVertexShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual HullShader* CreateHullShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual DomainShader* CreateDomainShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual PixelShader* CreatePixelShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual GeometryShader* CreateGeometryShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual ComputeShader* CreateComputeShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual SamplerState* CreateSamplerState(const SamplerStateDesc& Initializer) = 0;
	virtual BlendState* CreateBlendState() = 0;
	virtual ConstantBuffer* CreateConstantBuffer(unsigned int BufferSize) = 0;
	virtual VertexDeclaration* CreateVertexDeclaration(vector<VertexElement>& Elements) = 0;
	virtual RasterizerState* CreateRasterizerState(const RasterizerDesc& Initializer) = 0;
	virtual DepthStencilState* CreateDepthStencilState(const DepthStencilDesc& Initializer) = 0;
	virtual VertexBuffer* CreateVertexBuffer(const void * pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage) = 0;
	virtual IndexBuffer* CreateIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride, BUFFER_USAGE_FLAGS InUsage) = 0;

	virtual void SetVertexBuffer(VertexBuffer *vertexBuffer, unsigned int StartSlot, unsigned int Stride, unsigned int Offset) = 0;
	virtual void SetIndexBuffer(IndexBuffer *indexBuffer) = 0;
	virtual void SetViewport(ViewPort *pViewPort) = 0;
	virtual void SetBlendState(COMMON_BLEND_STATES state) = 0;
	virtual void SetDepthStencilState(COMMON_DEPTH_STENCIL_STATES state) = 0;
	virtual void SetRasterizerState(COMMON_RASTERIZER_STATES state) = 0;
	virtual void SetSamplerState(COMMON_SAMPLER_STATES state) = 0;
	virtual void SetRenderTarget(Texture2D *renderTarget, Texture2D *depthStencil, bool clearRenderTarget = false, bool clearDepthStensil = false) = 0;

	virtual void Draw(unsigned int vertexCount) = 0;
	virtual void DrawIndexedPrimitive(unsigned int NumPrimitives, unsigned int StartIndex, int BaseVertexIndex) = 0;
};

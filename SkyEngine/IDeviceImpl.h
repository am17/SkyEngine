#pragma once
#include <Windows.h>
#include "Shader.h"
#include "SamplerStateDesc.h"
#include "ConstantBuffer.h"
#include "RasterizerDesc.h"
#include "DepthStencilDesc.h"
#include <vector>

using namespace std;

class IDeviceImpl
{
public:
	virtual ~IDeviceImpl() {}
	virtual bool Init() = 0;
	virtual void *CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount = 1, unsigned int multiSampleQuality = 0) = 0;
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
};

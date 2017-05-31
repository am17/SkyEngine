#pragma once
#include "DeviceResourceImpl.h"

struct DevVertexShader
{
	//DeviceConstantBuffer ConstantBuffers[16];
	int ShaderResources[128];
	int SamplarStates[16];
};

struct DevHullShader
{
	//DeviceConstantBuffer ConstantBuffers[16];
	int ShaderResources[128];
	int SamplarStates[16];
};

struct DevDomainShader
{
	//DeviceConstantBuffer ConstantBuffers[16];
	int ShaderResources[128];
	int SamplarStates[16];
};

struct DevGeometryShader
{
	//DeviceConstantBuffer ConstantBuffers[16];
	int ShaderResources[128];
	int SamplarStates[16];
};

struct DevPixelShader
{
	//DeviceConstantBuffer ConstantBuffers[16];
	int ShaderResources[128];
	int SamplarStates[16];
};

struct DevComputeShader
{
	//DeviceConstantBuffer ConstantBuffers[16];
	int ShaderResources[128];
	int SamplarStates[16];
};

struct DeviceRenderData
{
	//IVertexBufferImpl VertexBuffers[32];
	//IIndexBufferImpl *indexBuffer;
	//InputLayout *inputLayout;
	//int PrimitiveTopology;
	//int ResterizerState;
	//int BlendState;
	//DevVertexShader vertexShader;
	//DevHullShader hullShader;
	//DevDomainShader domainShader;
	//DevGeometryShader geometryShader;
	//DevPixelShader pixelShader;
	//DevComputeShader computeShader;
};
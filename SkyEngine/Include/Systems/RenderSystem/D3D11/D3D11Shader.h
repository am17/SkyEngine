#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Systems\RenderSystem\Shader.h"

class D3D11VertexShader : public VertexShader
{
public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> Resource;
	void* Code;
	size_t ByteCodeLength;
};

class D3D11HullShader : public HullShader
{
public:
	Microsoft::WRL::ComPtr<ID3D11HullShader> Resource;
};

class D3D11DomainShader : public DomainShader
{
public:
	Microsoft::WRL::ComPtr<ID3D11DomainShader> Resource;
};

class D3D11PixelShader : public PixelShader
{
public:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> Resource;
};

class D3D11GeometryShader : public GeometryShader
{
public:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> Resource;
};

class D3D11ComputeShader : public ComputeShader
{
public:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> Resource;
};


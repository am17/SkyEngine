#include "stdafx.h"
#include "Systems\RenderSystem\ShaderFactory.h"
#include <string>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

ShaderFactory::ShaderFactory(IDeviceImpl* aDevice)
	:mDevice(aDevice)
{
}


ShaderFactory::~ShaderFactory()
{
}

Shader * ShaderFactory::GetShader(const wchar_t * name, SHADER_TYPE shaderType, const char * pEntryPoint)
{
	shared_ptr<Shader> shader = nullptr;

	unordered_map<const wchar_t*, shared_ptr<Shader>>::const_iterator it = mShaderCache.find(name);

	if (mShaderCache.end() == it)
	{
		//SkyEngine\TestApp\Shaders
		wstring FileName(L"Shaders/");
		FileName.append(name);
		FileName.append(L".hlsl");

		shader = shared_ptr<Shader>(CreateShaderFromFile(FileName.c_str(), shaderType, pEntryPoint));
		mShaderCache[name] = shader;
	}
	else
	{
		shader = it->second;
	}

	return shader.get();
}

shared_ptr<ShaderFactory> ShaderFactory::p_instance;

ShaderFactory & ShaderFactory::GetInstance(IDeviceImpl* aDevice)
{
	if (!p_instance) {
		p_instance = make_shared<ShaderFactory>(aDevice);
	}
	return *p_instance.get();
}

Shader * ShaderFactory::CreateShaderFromFile(const wchar_t * filename, SHADER_TYPE shaderType, const char * pEntryPoint)
{
	Shader *shader = nullptr;

	switch (shaderType)
	{
	case SHADER_TYPE::VERTEX_SHADER:
		{
			ID3D10Blob* pShaderBuffer = CompileShader(filename, pEntryPoint, "vs_5_0");
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = mDevice->CreateVertexShader(ByteCode, ByteCodeLength);
		}
		break;
	case SHADER_TYPE::HULL_SHADER:
		{
			ID3D10Blob* pShaderBuffer = CompileShader(filename, pEntryPoint, "hs_5_0");
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = mDevice->CreateHullShader(ByteCode, ByteCodeLength);
		}
		break;
	case SHADER_TYPE::DOMAIN_SHADER:
		{
			ID3D10Blob* pShaderBuffer = CompileShader(filename, pEntryPoint, "ds_5_0");
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = mDevice->CreateDomainShader(ByteCode, ByteCodeLength);
		}
		break;
	case SHADER_TYPE::PIXEL_SHADER:
		{
			ID3D10Blob* pShaderBuffer = CompileShader(filename, pEntryPoint, "ps_5_0");
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = mDevice->CreatePixelShader(ByteCode, ByteCodeLength);
		}
		break;
	case SHADER_TYPE::GEOMETRY_SHADER:
		{
			ID3D10Blob* pShaderBuffer = CompileShader(filename, pEntryPoint, "gs_5_0");
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = mDevice->CreateGeometryShader(ByteCode, ByteCodeLength);
		}
		break;
	case SHADER_TYPE::COMPUTE_SHADER:
		{
			ID3D10Blob* pShaderBuffer = CompileShader(filename, pEntryPoint, "cs_5_0");
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = mDevice->CreateComputeShader(ByteCode, ByteCodeLength);
		}
		break;
	default:
		break;
	}

	assert(shader != nullptr);

	return shader;
}

ID3D10Blob*  ShaderFactory::CompileShader(const wchar_t * filename, const char * pEntryPoint, const char* pTarget)
{
	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMessage = nullptr;

	HRESULT result = D3DCompileFromFile(filename, nullptr, nullptr, pEntryPoint, pTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pShaderBuffer, &pErrorMessage);

	if (FAILED(result))
	{
		printError(pErrorMessage);
	}

	return pShaderBuffer;
}

void ShaderFactory::printError(ID3D10Blob* pErrorMessage)
{
	if (pErrorMessage)
	{
		char* compileErrors = static_cast<char*>(pErrorMessage->GetBufferPointer());
		printf("shader compile Error: %s\n", compileErrors);
		pErrorMessage->Release();
		pErrorMessage = nullptr;
	}
	else
	{
		printf("missing shader file\n");
	}
}

#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <mbstring.h>"

#pragma comment(lib, "D3DCompiler.lib")

ShaderFactory::ShaderFactory(Device* aDevice)
	:_pDevice(aDevice)
{
}

ShaderFactory::~ShaderFactory()
{
	for (ShadersCollection::const_iterator it = _shaders.begin(); it != _shaders.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
}

Shader* ShaderFactory::getShader(const wchar_t* filename, sky::EShaderType shaderType, const char* pEntryPoint)
{
	//size_t origsize = wcslen(filename) + 1;
	//size_t convertedChars = 0;

	//const size_t newsize = origsize * 2;
	//char *nstring = new char[newsize];
	//wcstombs_s(&convertedChars, nstring, newsize, filename, _TRUNCATE);

	//DWORD key = filename | pEntryPoint;

	ShadersCollection::const_iterator it = _shaders.find(filename);

	if (_shaders.end() == it)
	{
		Shader* shader = getShaderFromFile(filename, shaderType, pEntryPoint);

		_shaders[filename] = shader;

		return shader;
	}
	else
	{
		return it->second;
	}
}

Shader * ShaderFactory::getShaderFromFile(const wchar_t* filename, sky::EShaderType shaderType, const char* pEntryPoint)
{
	Shader* shader = nullptr;
	switch (shaderType)
	{
	case sky::EShaderType::STVertex:
		{
			ID3D10Blob* pShaderBuffer = compileAsVertexShader(filename, pEntryPoint);
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = _pDevice->createVertexShader(ByteCode, ByteCodeLength);
		}
		break;
	case sky::EShaderType::STHull:
		break;
	case sky::EShaderType::STDomain:
		break;
	case sky::EShaderType::STPixel:
		{
			ID3D10Blob* pShaderBuffer = compileAsPixelShader(filename, pEntryPoint);
			const void* ByteCode = pShaderBuffer->GetBufferPointer();
			size_t ByteCodeLength = pShaderBuffer->GetBufferSize();
			shader = _pDevice->createPixelShader(ByteCode, ByteCodeLength);
		}
		break;
	case sky::EShaderType::STGeometry:
		break;
	case sky::EShaderType::STCompute:
		break;
	default:
		break;
	}

	assert(shader != nullptr);

	return shader;
}

ID3D10Blob* ShaderFactory::compileAsVertexShader(const wchar_t* filename, const char* pEntryPoint)
{
	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMessage = nullptr;

	const char* pTarget = "vs_4_0";

	HRESULT result = D3DCompileFromFile(filename, nullptr, nullptr, pEntryPoint, pTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pShaderBuffer, &pErrorMessage);

	if (FAILED(result))
	{
		printError(pErrorMessage);
	}

	assert(pShaderBuffer != nullptr);

	return pShaderBuffer;
}

ID3D10Blob* ShaderFactory::compileAsPixelShader(const wchar_t* filename, const char* pEntryPoint)
{
	ID3D10Blob* pShaderBuffer = nullptr;
	ID3D10Blob* pErrorMessage = nullptr;

	const char* pTarget = "ps_4_0";

	HRESULT result = D3DCompileFromFile(filename, nullptr, nullptr, pEntryPoint, pTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pShaderBuffer, &pErrorMessage);

	if (FAILED(result))
	{
		printError(pErrorMessage);
	}

	assert(pShaderBuffer != nullptr);

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

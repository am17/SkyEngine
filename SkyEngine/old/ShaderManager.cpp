#include "stdafx.h"
#include "ShaderManager.h"
#include <d3dcompiler.h>
#include "IEntity.h"
#include "Log.h"

#pragma comment(lib, "D3DCompiler.lib")

using namespace scene;

namespace sky
{
	ShaderManager::ShaderManager(IDevice *device)
		:m_device(device),
		include(nullptr)
	{
		//BASE INPUT LAYOUT
		m_baseVertexDeclaration[0] = { "POSITION", 0, ELEMENT_FORMAT::FLOAT3, 0, false };
		m_baseVertexDeclaration[1] = { "NORMAL", 0, ELEMENT_FORMAT::FLOAT3, 0, false };
		m_baseVertexDeclaration[2] = { "TANGENT", 0, ELEMENT_FORMAT::FLOAT3, 0, false };
		m_baseVertexDeclaration[3] = { "TEXCOORD", 0, ELEMENT_FORMAT::FLOAT2, 0, false };
		m_baseVertexDeclaration[4] = { "COLOR", 0, ELEMENT_FORMAT::FLOAT4, 0, false };

		m_colorVertexDeclaration[0] = { "POSITION", 0, ELEMENT_FORMAT::FLOAT3, 0, false };
		m_colorVertexDeclaration[1] = { "COLOR", 0, ELEMENT_FORMAT::FLOAT4, 0, false };

		include = new IncludeFX("");
	}


	ShaderManager::~ShaderManager()
	{
		for (unordered_map<EVertexShader, IVertexShader*>::const_iterator it = m_vertexShadersMap.begin(); it != m_vertexShadersMap.end(); ++it)
		{
			IVertexShader *shader = (*it).second;
			if (shader)
			{
				delete shader;
				shader = nullptr;
			}
		}

		for (unordered_map<EPixelShader, IPixelShader*>::const_iterator it = m_pixelShadersMap.begin(); it != m_pixelShadersMap.end(); ++it)
		{
			IPixelShader *shader = (*it).second;
			if (shader)
			{
				delete shader;
				shader = nullptr;
			}
		}

		if (include)
		{
			delete include;
		}
	}

	bool ShaderManager::initShadersCollection()
	{
		assert(m_device != nullptr);

		initBaseShader();
		//initNormalMappingShader();
		//initSpecularMappingShader();
		//initReflectionShader();
		initTerrainShader();
		//initSkyboxShader();
		initColorShader();

		return true;
	}

	IVertexShader * ShaderManager::createVertexShaderInstance(const WCHAR* vsFilename, const char* pEntryPoint, const char* pTarget, size_t contantBufferCount, size_t vertexBufferCount) const
	{
		CompileResult compRes = compileFromFile(vsFilename, pEntryPoint, pTarget);

		if (compRes.isSucceeded)
		{
			return m_device->createVertexShader(compRes.pShaderBuffer->GetBufferPointer(), compRes.pShaderBuffer->GetBufferSize(), contantBufferCount, vertexBufferCount);
		}

		return nullptr;
	}

	IPixelShader * ShaderManager::createPixelShaderInstance(const WCHAR* vsFilename, const char* pEntryPoint, const char* pTarget, size_t contantBufferCount, size_t shaderResourceCount) const
	{
		CompileResult compRes = compileFromFile(vsFilename, pEntryPoint, pTarget);

		if (compRes.isSucceeded)
		{
			return m_device->createPixelShader(compRes.pShaderBuffer->GetBufferPointer(), compRes.pShaderBuffer->GetBufferSize(), contantBufferCount, shaderResourceCount);
		}

		return nullptr;
	}

	void ShaderManager::initBaseShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"baseVS.hlsl", "ColorVertexShader", "vs_4_0", 2, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_baseVertexDeclaration, 5, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));
			vertexShader->setConstantBufferSize(1, sizeof(SCameraBuffer));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_BASE, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"basePS.hlsl", "ColorPixelShader", "ps_4_0", 1, 1);

		if (pixelShader)
		{
			pixelShader->setConstantBufferSize(0, sizeof(CBPerFrame));

			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_BASE, pixelShader));
		}
	}

	void ShaderManager::initNormalMappingShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"normalMapVS.hlsl", "main", "vs_4_0", 2, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_baseVertexDeclaration, 5, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));
			vertexShader->setConstantBufferSize(1, sizeof(SCameraBuffer));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_NORMAL_MAPPING, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"normalMapPS.hlsl", "main", "ps_4_0", 1, 2);

		if (pixelShader)
		{
			pixelShader->setConstantBufferSize(0, sizeof(CBPerFrame));

			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_NORMAL_MAPPING, pixelShader));
		}
	}

	void ShaderManager::initSpecularMappingShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"specularMapVS.hlsl", "main", "vs_4_0", 2, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_baseVertexDeclaration, 5, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));
			vertexShader->setConstantBufferSize(1, sizeof(SCameraBuffer));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_SPECULAR_MAPPING, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"specularMapPS.hlsl", "main", "ps_4_0", 1, 3);

		if (pixelShader)
		{
			pixelShader->setConstantBufferSize(0, sizeof(CBPerFrame));

			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_SPECULAR_MAPPING, pixelShader));
		}
	}

	void ShaderManager::initReflectionShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"reflectionVS.hlsl", "main", "vs_4_0", 2, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_baseVertexDeclaration, 5, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));
			vertexShader->setConstantBufferSize(1, sizeof(SCameraBuffer));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_REFLECTION, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"reflectionPS.hlsl", "main", "ps_4_0", 1, 2);

		if (pixelShader)
		{
			pixelShader->setConstantBufferSize(0, sizeof(CBPerFrame));

			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_REFLECTION, pixelShader));
		}
	}

	void ShaderManager::initTerrainShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"terrainVS.hlsl", "main", "vs_4_0", 2, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_baseVertexDeclaration, 5, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));
			vertexShader->setConstantBufferSize(1, sizeof(SCameraBuffer));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_TERRAIN, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"terrainPS.hlsl", "main", "ps_4_0", 1, 2);

		if (pixelShader)
		{
			pixelShader->setConstantBufferSize(0, sizeof(CBPerFrame));

			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_TERRAIN, pixelShader));
		}
	}

	void ShaderManager::initSkyboxShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"skyboxVS.hlsl", "main", "vs_4_0", 2, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_baseVertexDeclaration, 5, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));
			vertexShader->setConstantBufferSize(1, sizeof(SCameraBuffer));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_SKYBOX, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"skyboxPS.hlsl", "main", "ps_4_0", 0, 1);

		if (pixelShader)
		{
			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_SKYBOX, pixelShader));
		}
	}

	void ShaderManager::initColorShader()
	{
		IVertexShader *vertexShader = createVertexShaderInstance(L"colorVS.hlsl", "main", "vs_4_0", 1, 1);

		if (vertexShader)
		{
			VertexInputLayout* layout = m_device->getInputLayout(m_colorVertexDeclaration, 2, vertexShader->getByteCode(), vertexShader->getByteCodeLength());

			vertexShader->setInputSignature(layout);

			vertexShader->setConstantBufferSize(0, sizeof(CBPerObject));

			m_vertexShadersMap.insert(pair<EVertexShader, IVertexShader*>(EVertexShader::VS_COLOR, vertexShader));
		}

		IPixelShader *pixelShader = createPixelShaderInstance(L"colorPS.hlsl", "main", "ps_4_0", 0, 0);

		if (pixelShader)
		{
			m_pixelShadersMap.insert(pair<EPixelShader, IPixelShader*>(EPixelShader::PS_COLOR, pixelShader));
		}
	}

	CompileResult ShaderManager::compileFromFile(const WCHAR* vsFilename, const char* pEntryPoint, const char* pTarget) const
	{
		bool isSucceeded = false;

		ID3D10Blob* pShaderBuffer = nullptr;
		ID3D10Blob* pErrorMessage = nullptr;

		HRESULT result = D3DCompileFromFile(vsFilename, nullptr, include, pEntryPoint, pTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pShaderBuffer, &pErrorMessage);
		if (FAILED(result))
		{
			isSucceeded = false;

			if (pErrorMessage)
			{
				//Error compiling shader

				char* compileErrors = static_cast<char*>(pErrorMessage->GetBufferPointer());

				Log::write(compileErrors);

				//size_t bufferSize = pErrorMessage->GetBufferSize();

				pErrorMessage->Release();
				pErrorMessage = nullptr;

				//Error compiling shader
			}
			else
			{
				char Description[128];
				size_t charNum;
				int error = wcstombs_s(&charNum, Description, 128, vsFilename, 128);

				char *fileName = new char[128];

				strcpy_s(fileName, 128, Description);

				Log::write(fileName);
				Log::write("Missing shader file");

				if (fileName)
				{
					delete fileName;
					fileName = nullptr;
				}
			}
		}
		else
		{
			isSucceeded = true;
		}

		assert(isSucceeded);

		/*if (pShaderBuffer)
		{
			pShaderBuffer->Release();
			pShaderBuffer = nullptr;
		}*/

		return{ pShaderBuffer, isSucceeded };
	}

	IVertexShader * ShaderManager::getVertexShader(EVertexShader shader) const
	{
		unordered_map<EVertexShader, IVertexShader*>::const_iterator it = m_vertexShadersMap.find(shader);

		if (it != m_vertexShadersMap.end())
		{
			return it->second;
		}

		return nullptr;
	}
	IPixelShader * ShaderManager::getPixelShader(EPixelShader shader) const
	{
		unordered_map<EPixelShader, IPixelShader*>::const_iterator it = m_pixelShadersMap.find(shader);

		if (it != m_pixelShadersMap.end())
		{
			return it->second;
		}

		return nullptr;
	}
}

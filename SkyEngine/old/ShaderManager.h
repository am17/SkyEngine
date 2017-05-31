#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "IShader.h"
#include "IDevice.h"
#include "ShaderTypes.h"
#include "IncludeFX.h"

using namespace std;

namespace sky
{
	struct CompileResult
	{
		ID3D10Blob* pShaderBuffer;//TO DO: днаюбхрэ Realese
		bool isSucceeded;
	};

	class ShaderManager
	{
	public:
		ShaderManager(IDevice *m_device);
		~ShaderManager();
		bool initShadersCollection();
		CompileResult compileFromFile(const WCHAR* vsFilename, const char* pEntryPoint, const char* pTarget) const;
		IVertexShader *getVertexShader(EVertexShader shader) const;
		IPixelShader *getPixelShader(EPixelShader shader) const;
	private:
		IVertexShader *createVertexShaderInstance(const WCHAR* vsFilename, const char* pEntryPoint, const char* pTarget, size_t contantBufferCount, size_t vertexBufferCount) const;
		IPixelShader *createPixelShaderInstance(const WCHAR* vsFilename, const char* pEntryPoint, const char* pTarget, size_t contantBufferCount, size_t shaderResourceCount) const;
		void initBaseShader();
		void initNormalMappingShader();
		void initSpecularMappingShader();
		void initReflectionShader();
		void initTerrainShader();
		void initSkyboxShader();
		void initColorShader();

		IDevice *m_device;

		unordered_map<EVertexShader, IVertexShader*> m_vertexShadersMap;
		unordered_map<EPixelShader, IPixelShader*> m_pixelShadersMap;

		VertexLayoutDesc m_baseVertexDeclaration[6];

		VertexLayoutDesc m_colorVertexDeclaration[2];

		IncludeFX *include;
	};
}


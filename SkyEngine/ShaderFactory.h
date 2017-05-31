#pragma once
#include <unordered_map>
#include "Device.h"
#include "ShaderTypes.h"
#include <d3d11.h>

using namespace std;

class ShaderFactory
{
public:
	ShaderFactory(Device* aDevice);
	~ShaderFactory();
	Shader* getShader(const wchar_t* filename, sky::EShaderType shaderType, const char* pEntryPoint = "main");
private:
	Shader *getShaderFromFile(const wchar_t* filename, sky::EShaderType shaderType, const char* pEntryPoint);
	ID3D10Blob* compileAsVertexShader(const wchar_t* filename, const char* pEntryPoint);
	ID3D10Blob* compileAsPixelShader(const wchar_t* filename, const char* pEntryPoint);
	void printError(ID3D10Blob* pErrorMessage);

	Device* _pDevice;
	typedef unordered_map<const wchar_t*, Shader*> ShadersCollection;
	ShadersCollection _shaders;
};


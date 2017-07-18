#pragma once
#include "IDeviceImpl.h"
#include "Shader.h"
#include <unordered_map>
#include "common.h"
#include <memory>
#include <d3d11.h>

using namespace std;

class SKYENGINEDLL ShaderFactory
{
public:
	ShaderFactory(IDeviceImpl* aDevice);
	~ShaderFactory();
	Shader* GetShader(const wchar_t* name, SHADER_TYPE shaderType, const char* pEntryPoint = "main");
private:
	Shader *CreateShaderFromFile(const wchar_t* filename, SHADER_TYPE shaderType, const char* pEntryPoint);
	ID3D10Blob* CompileShader(const wchar_t * filename, const char * pEntryPoint, const char* pTarget);
	void printError(ID3D10Blob* pErrorMessage);

	IDeviceImpl* mDevice;
	unordered_map<const wchar_t*, shared_ptr<Shader>> mShaderCache;
};


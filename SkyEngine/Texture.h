#pragma once
#include "DeviceResourceImpl.h"
#include "ShaderTypes.h"
#include "common.h"

class Texture
{
public:
	Texture(ITextureImpl *aImpl);
	~Texture();
	SKYENGINEDLL void saveToFile(const wchar_t* fileName);
	SKYENGINEDLL void saveToFile();
	SKYENGINEDLL void bind(sky::EShaderType shaderType, unsigned int startSlot = 0);
	void bindAsRenderTarget();
private:
	ITextureImpl *_impl;
};


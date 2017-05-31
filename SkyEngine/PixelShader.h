#pragma once
#include "RenderResource.h"

class PixelShader
{
public:
	PixelShader(IPixelShaderImpl *aShaderImpl);
	~PixelShader();
	void bind();
private:
	IPixelShaderImpl*_pShaderImpl;
};


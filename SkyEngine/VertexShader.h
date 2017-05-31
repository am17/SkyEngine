#pragma once
#include "RenderResource.h"

class VertexShader
{
public:
	VertexShader(IVertexShaderImpl *aShaderImpl);
	~VertexShader();
	void bind();
private:
	IVertexShaderImpl *_pShaderImpl;
};


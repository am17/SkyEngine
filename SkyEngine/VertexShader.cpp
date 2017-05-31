#include "stdafx.h"
#include "VertexShader.h"


VertexShader::VertexShader(IVertexShaderImpl *aShaderImpl)
	: _pShaderImpl(aShaderImpl)
{
}


VertexShader::~VertexShader()
{
	if (_pShaderImpl)
	{
		delete _pShaderImpl;
		_pShaderImpl = nullptr;
	}
}

void VertexShader::bind()
{
	_pShaderImpl->bind();
}

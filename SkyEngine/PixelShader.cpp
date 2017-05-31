#include "stdafx.h"
#include "PixelShader.h"


PixelShader::PixelShader(IPixelShaderImpl *aShaderImpl)
	: _pShaderImpl(aShaderImpl)
{
}


PixelShader::~PixelShader()
{
	if (_pShaderImpl)
	{
		delete _pShaderImpl;
		_pShaderImpl = nullptr;
	}
}


void PixelShader::bind()
{
	_pShaderImpl->bind();
}

#pragma once
#include "DeviceResourceImpl.h"

class Shader
{
public:
	Shader(IShaderImpl *aShaderImpl)
		:_pShaderImpl(aShaderImpl)
	{
	}
	virtual ~Shader()
	{
		if (_pShaderImpl)
		{
			delete _pShaderImpl;
			_pShaderImpl = nullptr;
		}
	}
	void bind()
	{
		_pShaderImpl->bind();
	}
	const void* getByteCode() const { return _pShaderImpl->getByteCode(); }
	size_t getCodeLength() const { return _pShaderImpl->getCodeLength(); }
private:
	IShaderImpl*_pShaderImpl;
};
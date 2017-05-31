#include "stdafx.h"
#include "InputLayout.h"


InputLayout::InputLayout(IInputLayoutImpl *aImpl)
	: _pImpl(aImpl)
{
}


InputLayout::~InputLayout()
{
	if (_pImpl)
	{
		delete _pImpl;
		_pImpl = nullptr;
	}
}

void InputLayout::bind()
{
	_pImpl->bind();
}

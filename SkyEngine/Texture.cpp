#include "stdafx.h"
#include "Texture.h"
#include <stdio.h>

Texture::Texture(ITextureImpl *aImpl)
	:_impl(aImpl)
{
}

Texture::~Texture()
{
	if (_impl)
	{
		delete _impl;
		_impl = nullptr;
	}
}

void Texture::bind(sky::EShaderType shaderType, unsigned int startSlot)
{
	_impl->bind(shaderType, startSlot);
}

void Texture::resolve()
{
	_impl->resolve();
}

void Texture::saveToFile(const wchar_t* fileName)
{
	_impl->saveToFile(fileName);
}

void Texture::saveToFile()
{
	//_impl->saveToFile(L"Screenshots/screen.dds");
	//printf("screenSaved\n");
}

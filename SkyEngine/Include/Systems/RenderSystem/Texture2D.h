#pragma once
#include "Resource.h"

class Texture2D : public Resource
{
public:
	Texture2D() {}
	virtual ~Texture2D() {}
	virtual bool IsMultisampled() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
};
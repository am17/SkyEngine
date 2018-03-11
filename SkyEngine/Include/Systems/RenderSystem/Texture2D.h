#pragma once
#include "Resource.h"

class Texture2D : public Resource
{
public:
	Texture2D() {}
	~Texture2D() {}
	virtual bool IsMultisampled() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual bool Create(int width, int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality) = 0;
};
#pragma once
#include <Windows.h>

class IDeviceImpl
{
public:
	virtual ~IDeviceImpl() {}
	virtual bool Init() = 0;
	virtual void *CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount = 1, unsigned int multiSampleQuality = 0) = 0;
};

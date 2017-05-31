#pragma once
#include "ShaderTypes.h"

class IVertexBufferImpl
{
public:
	virtual ~IVertexBufferImpl() {}
	virtual bool create(const void *pData, unsigned int elementsCount, unsigned int stride, unsigned int offset = 0) = 0;
	virtual void bind(unsigned int startSlot) = 0;
};

class IIndexBufferImpl
{
public:
	virtual ~IIndexBufferImpl() {}
	virtual bool create(const void *pData, unsigned int elementsCount, unsigned int stride) = 0;
	virtual void bind(unsigned int offset) = 0;
};


class IContantBufferImpl
{
public:
	virtual ~IContantBufferImpl() {}
	virtual bool create(unsigned int bufferSize, bool isDynamic) = 0;
	virtual void bind(sky::EShaderType shaderType, unsigned int startSlot) = 0;
	virtual void update(const void *aSrcData) = 0;
};

class IVertexShaderImpl
{
public:
	virtual ~IVertexShaderImpl() {}
	virtual bool create(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual void bind() = 0;
};

class IPixelShaderImpl
{
public:
	virtual ~IPixelShaderImpl() {}
	virtual bool create(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual void bind() = 0;
};
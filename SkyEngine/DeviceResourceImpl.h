#pragma once
#include "ShaderTypes.h"
#include "VertexLayoutDesc.h"

class IVertexBufferImpl
{
public:
	virtual ~IVertexBufferImpl() {}
	virtual bool create(const void *pData, unsigned int elementsCount, unsigned int stride, unsigned int offset = 0) = 0;
	virtual void bind(unsigned int startSlot) = 0;
};

enum class INDEX_BUFFER_FORMAT
{
	IB_FORMAT_16 = 0,
	IB_FORMAT_32 = 1
};

enum class EConstantBufferType
{
	//Cannot Update: Level BSPs
	CBT_Forever,
	//UpdateSubResource: Characters
	CBT_LongLived,
	//CTransientBuffer: UI/Text 
	CBT_Transient,
	//Map/NO_OVERWRITE: Particles 
	CBT_Temporary,
	//Map/DISCARD: Material Props
	CBT_Constant
};

class IIndexBufferImpl
{
public:
	virtual ~IIndexBufferImpl() {}
	virtual bool create(const void *pData, unsigned int elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format) = 0;
	virtual void bind(unsigned int offset) = 0;
	virtual unsigned int getIndexCount() const = 0;
};


class IContantBufferImpl
{
public:
	virtual ~IContantBufferImpl() {}
	virtual bool create(unsigned int bufferSize, EConstantBufferType type) = 0;
	virtual void bind(sky::EShaderType shaderType, unsigned int startSlot) = 0;
	virtual void update(void *aSrcData) = 0;
};

class IInputLayoutImpl
{
public:
	virtual ~IInputLayoutImpl() {}
	virtual bool create(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual void bind() = 0;
};

class IShaderImpl
{
public:
	virtual ~IShaderImpl() {}
	virtual bool create(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual void bind() = 0;
	virtual const void* getByteCode() const = 0;
	virtual size_t getCodeLength() const = 0;
};

class ITextureImpl
{
public:
	virtual ~ITextureImpl(){}
	virtual bool createFromFile(wchar_t* filename) = 0;
	virtual bool createTexture1D(unsigned int width, const void* data) = 0;
	virtual bool createTexture2D(unsigned int width, unsigned int height, const void* data) = 0;
	virtual void bind(sky::EShaderType shaderType, unsigned int startSlot) = 0;
	virtual void unbind(sky::EShaderType shaderType, unsigned int startSlot) = 0;
	virtual void saveToFile(const wchar_t* fileName) = 0;
};

class IRenderTargetImpl
{
public:
	virtual ~IRenderTargetImpl(){}
	virtual bool create(unsigned int width, unsigned int height) = 0;
	virtual ITextureImpl *getTextureImpl() const = 0;
	virtual void clear() = 0;
};

enum class PRIMITIVE_TOPOLOGY
{
	TRIANGLELIST = 0,
	TRIANGLESTRIP = 1,
	LINELIST = 2
};
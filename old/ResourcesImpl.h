#pragma once
#include "PixelFormat.h"
#include "VertexElement.h"
#include <float.h>

class ResourceImpl 
{
public:
	ResourceImpl() {}
	virtual ~ResourceImpl() {}
};

class SamplerStateImpl : public ResourceImpl {};
class RasterizerStateImpl : public ResourceImpl {};
class DepthStencilStateImpl : public ResourceImpl {};
class BlendStateImpl : public ResourceImpl {};

class VertexDeclarationImpl : public ResourceImpl {};
class BoundShaderStateImpl : public ResourceImpl {};

class ShaderImpl : public ResourceImpl {};

class VertexShaderImpl : public ShaderImpl {};
class HullShaderImpl : public ShaderImpl {};
class DomainShaderImpl : public ShaderImpl {};
class PixelShaderImpl : public ShaderImpl {};
class GeometryShaderImpl : public ShaderImpl {};
class ComputeShaderImpl : public ShaderImpl {};

enum class BUFFER_USAGE_FLAGS
{
	STATIC = 0X0001,
	DYNAMIC = 0X0002,
	VOLATILE = 0X0004,
	UNORDERED_ACCESS = 0X0008,
	BYTE_ADDRESS_BUFFER = 0X0020,
	UAVCOUNTER = 0X0040,
	STREAM_OUTPUT = 0X0080,
	DRAW_INDIRECT = 0X0100,
	SHADER_RESOURCE = 0X0200,
	KEEP_CPU_ACCESSIBLE = 0X0400,
	ZERO_STRIDE = 0X0800,
	FAST_VRAM = 0X1000,
	ANYDYNAMIC = (DYNAMIC | VOLATILE),
};

class IndexBufferImpl : public ResourceImpl
{
public:
	IndexBufferImpl(unsigned int stride, unsigned int size, BUFFER_USAGE_FLAGS usage):
		m_stride(stride), 
		m_size(size),
		m_usage(usage)
	{}
	unsigned int getStride() const { return m_stride; }
	unsigned int getSize() const { return m_size; }
	BUFFER_USAGE_FLAGS getUsage() const { return m_usage; }
private:
	unsigned int m_stride;
	unsigned int m_size;
	BUFFER_USAGE_FLAGS m_usage;
};

class VertexBufferImpl : public ResourceImpl
{
public:
	VertexBufferImpl(unsigned int size, BUFFER_USAGE_FLAGS usage):
		m_size(size), 
		m_usage(usage)
	{}
	unsigned int getSize() const { return m_size; }
	BUFFER_USAGE_FLAGS getUsage() const { return m_usage; }
private:
	unsigned int m_size;
	BUFFER_USAGE_FLAGS m_usage;
};

class TextureImpl : public ResourceImpl
{
public:
	TextureImpl(unsigned int numMips, unsigned int numSamples, PIXEL_FORMAT format, unsigned int flags):
		m_numMips(numMips), 
		m_numSamples(numSamples), 
		m_format(format), 
		m_flags(flags)
	{}

	unsigned int getNumMips() const { return m_numMips; }
	PIXEL_FORMAT getFormat() const { return m_format; }
	unsigned int getFlags() const { return m_flags; }
	unsigned int getNumSamples() const { return m_numSamples; }
	bool isMultisampled() const { return m_numSamples > 1; }
private:
	unsigned int m_numMips;
	unsigned int m_numSamples;
	PIXEL_FORMAT m_format;
	unsigned int m_flags;
};

class Texture2DImpl : public TextureImpl
{
public:
	Texture2DImpl(unsigned int sizeX, unsigned int sizeY, unsigned int numMips, unsigned int numSamples, PIXEL_FORMAT format, unsigned int flags):
		TextureImpl (numMips, numSamples, format, flags), m_sizeX(sizeX), m_sizeY(sizeY) {}
	unsigned int getSizeX() const { return m_sizeX; }
	unsigned int getSizeY() const { return m_sizeY; }
private:
	unsigned int m_sizeX;
	unsigned int m_sizeY;
};

class Texture2DArrayImpl : public TextureImpl
{
public:
	Texture2DArrayImpl(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int numMips, PIXEL_FORMAT format, unsigned int flags)
		:TextureImpl(numMips, 1, format, flags), m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ) {}
	unsigned int getSizeX() const { return m_sizeX; }
	unsigned int getSizeY() const { return m_sizeY; }
	unsigned int getSizeZ() const { return m_sizeZ; }
private:
	unsigned int m_sizeX;
	unsigned int m_sizeY;
	unsigned int m_sizeZ;
};

class Texture3DImpl : public TextureImpl
{
public:
	Texture3DImpl(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int numMips, PIXEL_FORMAT format, unsigned int flags)
		:TextureImpl(numMips, 1, format, flags), m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ) {}
	unsigned int getSizeX() const { return m_sizeX; }
	unsigned int getSizeY() const { return m_sizeY; }
	unsigned int getSizeZ() const { return m_sizeZ; }
private:
	unsigned int m_sizeX;
	unsigned int m_sizeY;
	unsigned int m_sizeZ;
};

class TextureCubeImpl : public TextureImpl
{
public:
	TextureCubeImpl(unsigned int size, unsigned int numMips, PIXEL_FORMAT format, unsigned int flags) :
		TextureImpl(numMips, 1, format, flags), m_size(size) {}
	unsigned int getSize() const { return m_size; }
private:
	unsigned int m_size;
};

class ViewportImpl : public ResourceImpl
{
public:
	ViewportImpl(): ResourceImpl() {}
	virtual ~ViewportImpl() {}

	virtual void* GetNativeSwapChain() const { return nullptr; }
	virtual void* GetNativeBackBufferTexture() const { return nullptr; }
	virtual void* GetNativeBackBufferRT() const { return nullptr; }
	virtual void* GetNativeWindow(void** AddParam = nullptr) const { return nullptr; }
	Texture2DImpl* GetBackBuffer() const { return nullptr; }
	virtual bool Present(bool bLockToVsync) { return false; }
};

class ShaderResourceViewImpl : public ResourceImpl {};

enum class PRIMITIVE_TOPOLOGY
{
	TRIANGLELIST = 0,
	TRIANGLESTRIP = 1,
	LINELIST = 2,
	QUADLIST = 3,
	POINTLIST = 4,
	CONTROL_POINT_PATCHLIST_1 = 5,
	CONTROL_POINT_PATCHLIST_2 = 6,
	UNKNOWN = -1
};

enum class SAMPLER_FILTER
{
	POINT,
	BILINEAR,
	TRILINEAR,
	ANISOTROPICPOINT,
	ANISOTROPICLINEAR
};

enum class SAMPLER_ADDRESS_MODE
{
	WRAP,
	CLAMP,
	MIRROR,
	BORDER
};

enum class SAMPLER_COMPARE_FUNCTION
{
	NEVER,
	LESS
};

struct SamplerStateDesc
{
	SamplerStateDesc()
		:AddressU(SAMPLER_ADDRESS_MODE::WRAP),
		AddressV(SAMPLER_ADDRESS_MODE::WRAP),
		AddressW(SAMPLER_ADDRESS_MODE::WRAP),
		MipBias(0),
		MaxAnisotropy(0),
		MinMipLevel(0),
		MaxMipLevel(FLT_MAX),
		BorderColor(0),
		SamplerComparisonFunction(SAMPLER_COMPARE_FUNCTION::NEVER)
	{}
	SAMPLER_FILTER Filter;
	SAMPLER_ADDRESS_MODE AddressU;
	SAMPLER_ADDRESS_MODE AddressV;
	SAMPLER_ADDRESS_MODE AddressW;
	int MipBias;
	int MaxAnisotropy;
	float MinMipLevel;
	float MaxMipLevel;
	unsigned int BorderColor;
	SAMPLER_COMPARE_FUNCTION SamplerComparisonFunction;
};

enum class RASTERIZER_FILL_MODE
{
	WIREFRAME,
	SOLID
};

enum class RASTERIZER_CULL_MODE
{
	NONE = 0,
	CW = 1,
	CCW = 2
};

struct RasterizerStateDesc
{
	RASTERIZER_FILL_MODE FillMode;
	RASTERIZER_CULL_MODE CullMode;
	float DepthBias;
	float SlopeScaleDepthBias;
	bool bAllowMSAA;
	bool bEnableLineAA;
};

enum class COMPARE_FUNCTION
{
	LESS,
	LESSEQUAL,
	GREATER,
	GREATEREQUAL,
	EQUAL,
	NOTEQUAL,
	NEVER,
	ALWAYS
};

enum class STENCIL_OP
{
	KEEP,
	ZERO,
	REPLACE,
	SATURATEDINCREMENT,
	SATURATEDDECREMENT,
	INVERT,
	INCREMENT,
	DECREMENT
};

struct DepthStencilStateDesc
{
	DepthStencilStateDesc():
		bEnableDepthWrite(true), 
		DepthTest(COMPARE_FUNCTION::LESSEQUAL),
		bEnableFrontFaceStencil(false),
		FrontFaceStencilTest(COMPARE_FUNCTION::ALWAYS),
		FrontFaceStencilFailStencilOp(STENCIL_OP::KEEP),
		FrontFaceDepthFailStencilOp(STENCIL_OP::KEEP),
		FrontFacePassStencilOp(STENCIL_OP::KEEP),
		bEnableBackFaceStencil(false),
		BackFaceStencilTest(COMPARE_FUNCTION::ALWAYS),
		BackFaceStencilFailStencilOp(STENCIL_OP::KEEP),
		BackFaceDepthFailStencilOp(STENCIL_OP::KEEP),
		BackFacePassStencilOp(STENCIL_OP::KEEP),
		StencilReadMask(0xFF),
		StencilWriteMask(0xFF)
	{}
	bool bEnableDepthWrite;
	COMPARE_FUNCTION DepthTest;

	bool bEnableFrontFaceStencil;
	COMPARE_FUNCTION FrontFaceStencilTest;
	STENCIL_OP FrontFaceStencilFailStencilOp;
	STENCIL_OP FrontFaceDepthFailStencilOp;
	STENCIL_OP FrontFacePassStencilOp;
	bool bEnableBackFaceStencil;
	COMPARE_FUNCTION BackFaceStencilTest;
	STENCIL_OP BackFaceStencilFailStencilOp;
	STENCIL_OP BackFaceDepthFailStencilOp;
	STENCIL_OP BackFacePassStencilOp;
	unsigned char StencilReadMask;
	unsigned char StencilWriteMask;
};

struct BlendStateDesc
{
	bool bUseIndependentRenderTargetBlendStates;
};

enum class RESOURCE_LOCK_MODE
{
	READONLY,
	WRITEONLY,
	NUM
};
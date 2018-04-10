#pragma once
#include "VertexElement.h"

class Resource
{
public:
	Resource() {}
	virtual ~Resource() {}
};

class SamplerState : public Resource {};
class BlendState : public Resource {};
class RasterizerState: public Resource {};
class DepthStencilState: public Resource {};
class VertexDeclaration : public Resource {};

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

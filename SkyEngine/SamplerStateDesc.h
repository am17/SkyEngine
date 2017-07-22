#pragma once

enum class TEXTURE_FILTER
{
	ANISOTROPIC,
	LINEAR,
	POINT
};

enum class SAMPLER_ADDRESS_MODE
{
	WRAP,
	BORDER,
	CLAMP,
	MIRROR
};

enum class COMPARISON_FUNCTION
{
	ALWAYS,
	LESS,
	LESSEQUAL,
	GREATER,
	GREATEREQUAL,
	EQUAL,
	NOTEQUAL,
	NEVER
};

struct SamplerStateDesc
{
	TEXTURE_FILTER Filter;
	SAMPLER_ADDRESS_MODE TextureAddress;
	COMPARISON_FUNCTION ComparisonFunction;
	unsigned int MaxAnisotropy;
	SamplerStateDesc(): MaxAnisotropy(1) {}
};
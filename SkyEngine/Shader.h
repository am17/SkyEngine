#pragma once
#include "Resource.h"

class Shader : public Resource {};

class VertexShader : public Shader {};
class HullShader : public Shader {};
class DomainShader : public Shader {};
class PixelShader : public Shader {};
class GeometryShader : public Shader {};
class ComputeShader : public Shader {};

enum class SHADER_TYPE
{
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	PIXEL_SHADER,
	GEOMETRY_SHADER,
	COMPUTE_SHADER
};

#pragma once

namespace sky
{
	enum class EShaderType
	{
		ST_Vertex = 0,
		ST_Hull = 1,
		ST_Domain = 2,
		ST_Pixel = 3,
		ST_Geometry = 4,
		ST_Compute = 5
	};

	enum class EVertexShader
	{
		VS_BASE = 1,
		VS_NORMAL_MAPPING = 2,
		VS_SPECULAR_MAPPING = 3,
		VS_REFLECTION = 4,
		VS_TERRAIN = 5,
		VS_SKYBOX = 6,
		VS_COLOR = 7
	};

	enum class EPixelShader
	{
		PS_BASE = 1,
		PS_NORMAL_MAPPING = 2,
		PS_SPECULAR_MAPPING = 3,
		PS_REFLECTION = 4,
		PS_TERRAIN = 5,
		PS_SKYBOX = 6,
		PS_COLOR = 7
	};
}
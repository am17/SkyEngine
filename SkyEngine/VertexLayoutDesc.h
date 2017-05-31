#pragma once

enum class ELEMENT_FORMAT
{
	FLOAT2 = 0,
	FLOAT3 = 1,
	FLOAT4 = 2
};

struct VertexLayoutDesc
{
	char* SemanticName;
	unsigned int SemanticIndex;
	ELEMENT_FORMAT Format;
	unsigned int InputSlot;
	bool ForInstance;
};
#pragma once

enum class VERTEX_ELEMENT_TYPE
{
	NONE,
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	PACKEDNORMAL,	
	UBYTE4,
	UBYTE4N,
	COLOR,
	SHORT2,
	SHORT4,
	SHORT2N,		
	HALF2,			
	HALF4,
	SHORT4N,		
	USHORT2,
	USHORT4,
	USHORT2N,		
	USHORT4N,		 
	URGB10A2N,		
	MAX
};

struct VertexElement
{
	unsigned char StreamIndex;
	unsigned char Offset;
	VERTEX_ELEMENT_TYPE Type;
	unsigned char AttributeIndex;
	unsigned short Stride;
	unsigned short UseInstanceIndex;
	VertexElement() {}
};

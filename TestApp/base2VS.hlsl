cbuffer ConstantBuffer: register(b0)
{
	matrix WVP;
	matrix worldMatrix;
}

struct VertexInputType
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	output.position = mul(input.position, WVP);

	output.normal = mul(input.normal, (float3x3)worldMatrix);

	output.normal = normalize(output.normal);

	output.tex = input.tex;

	return output;
}
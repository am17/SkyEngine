cbuffer MatrixBuffer: register(b0)
{
	matrix WVP;
	matrix worldMatrix;
};

cbuffer CameraBuffer: register(b1)
{
	float3 cameraPosition;
	float padding;
	matrix reflectionMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float4 reflectionPosition : TEXCOORD2;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	matrix reflectProjectWorld;

	input.position.w = 1.0f;

	output.position = mul(input.position, WVP);

	output.color = input.color;

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);

	output.normal = normalize(output.normal);

	float4 worldPosition = mul(input.position, worldMatrix);

	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
	reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

	output.reflectionPosition = mul(input.position, reflectProjectWorld);

	return output;
}

Texture2D shaderTexture: register(t0);
Texture2D reflectionTexture: register(t1);
SamplerState SampleType: register(s0);

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
	float SpecularPower;
	float4 SpecularColor;
	float3 halfWayVector;
	float padding;
};

cbuffer cbPerFrame: register(b0)
{
	DirectionalLight dirLight;
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

float4 main(PixelInputType input) : SV_TARGET
{
	float2 reflectTexCoord;

	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
	float4 reflectionColor = reflectionTexture.Sample(SampleType, input.tex);

	float4 color = lerp(textureColor, reflectionColor, 0.45f);

	return color;
}
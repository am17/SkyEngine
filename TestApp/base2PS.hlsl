Texture2D shaderTexture:register(t0);
SamplerState SampleType: register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
	float padding;
};

float4 calculateLambertModelLight(float3 normal, float4 textureColor, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	float4 Ia = light.Ambient;

	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = lightIntensity * light.Diffuse;

	return (Ia + Id) * textureColor;
}

float4 main(PixelInputType input) : SV_TARGET
{
	DirectionalLight light;
	light.Ambient = float4(0.2, 0.2, 0.2, 1);
	light.Diffuse = float4(1,1,1,1);
	light.Direction = float3(1, 0, 0);
	light.padding = 0;

	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

	return calculateLambertModelLight(input.normal, textureColor, light);
}
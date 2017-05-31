Texture2D shaderTexture: register(t0);
Texture2D normalMap: register(t1);
Texture2D specularMap: register(t2);
SamplerState SampleType: register(s0);

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
	float SpecularPower;
	float4 SpecularColor;
};

cbuffer cbPerFrame: register(b0)
{
	DirectionalLight dirLight;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

	float4 bumpMap = normalMap.Sample(SampleType, input.tex);

	bumpMap = (bumpMap * 2.0f) - 1.0f;

	float3 bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;

	bumpNormal = normalize(bumpNormal);

	float3 directionToLight = -dirLight.Direction;

	float lightIntensity = saturate(dot(bumpNormal, directionToLight));

	float4 color = saturate(dirLight.Diffuse * lightIntensity);

	color = color * textureColor;

	//color = lerp(textureColor, color, 0.15f);

	if (lightIntensity > 0.0f)
	{
		float4 specularIntensity = specularMap.Sample(SampleType, input.tex);

		//float3 reflectDirection = normalize(2 * lightIntensity * bumpNormal - directionToLight);
		float3 reflectDirection = reflect(-directionToLight, bumpNormal);

		float4 specular = pow(saturate(dot(reflectDirection, input.viewDirection)), dirLight.SpecularPower);

		specular = specular * specularIntensity;

		color = saturate(color + specular);
	}

	return color;
}
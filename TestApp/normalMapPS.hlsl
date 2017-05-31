Texture2D shaderTexture: register(t0);
Texture2D normalMap: register(t1);
SamplerState SampleType: register(s0);

cbuffer LightBuffer: register(b0)
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

    float4 bumpColor = normalMap.Sample(SampleType, input.tex);
	
	bumpColor = (bumpColor * 2.0f) - 1.0f;
	
	float3 bumpNormal = (bumpColor.x * input.tangent) + (bumpColor.y * input.binormal) + (bumpColor.z * input.normal);
	
	bumpNormal = normalize(bumpNormal);
	
	float3 lightDir = -lightDirection;
	
	float lightIntensity = saturate(dot(bumpNormal, lightDir));
	
	float4 color = saturate(diffuseColor * lightIntensity);
	
	color = color * textureColor;

	if (lightIntensity > 0.0f)
	{
		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		float3 reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		float4 specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		// Add the specular component last to the output color.
		color = saturate(color + specular);
	}
	
    return color;
}
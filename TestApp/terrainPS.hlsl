SamplerState SamplerRepeatMaxAniso: register(s0); 
SamplerState SamplerClampLinear: register(s1);

Texture2D LunarSurfaceTex: register(t0);
Texture2D NormalMapMapSRV: register(t1);
Texture2D LunarSurfaceTex1: register(t2);
Texture2D LunarSurfaceTex2: register(t3);

cbuffer cbPerFrame: register(b0)
{
	float3 g_TextureWorldOffset;
	int g_CoarseSampleSpacing;
}

struct MeshVertex
{
	float4 vPosition         : SV_Position;
	float2 vWorldXZ          : TEXCOORD0;
	float4 depthPosition     : TEXTURE1;
	uint   renderRargetIndex : SV_RenderTargetArrayIndex;
};

float2 worldXZtoHeightUV(float2 worldXZ)
{
	// [-8,8] -> [0,1]  TBD: Ought to depend on world size though.
	return worldXZ / 16 + 0.5;
}

float4 calculateLambertModelLight(float3 normal, float4 textureColor)
{
	const float3 lightDirection = float3(0.0f, 0.0f, -1.0f);

	float3 directionToLight = -lightDirection;

	float4 ambientColor = float4(0.15f, 0.15f, 0.15f, 1.0f);
	float4 diffuseColor = float4(1, 1, 1, 1);

	float4 Ia = ambientColor;

	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);

	float4 Id = lightIntensity * diffuseColor;

	return (Ia + Id) * textureColor;
}

float4 getDepthColor(float4 depthPosition)
{
	float depthValue = (depthPosition.z / depthPosition.w);

	return float4(depthValue, depthValue, depthValue, 1.0f);
}

float4 main(MeshVertex input) : SV_TARGET
{
	float2 texUV = input.vWorldXZ + 2 * float2(g_TextureWorldOffset.x, -g_TextureWorldOffset.z);

	float4 macroDetail = LunarSurfaceTex.Sample(SamplerRepeatMaxAniso, texUV);

	float2 texUV2 = worldXZtoHeightUV(input.vWorldXZ + 2 * float2(g_TextureWorldOffset.x, -g_TextureWorldOffset.z));

	float3 bumpMap = NormalMapMapSRV.Sample(SamplerClampLinear, texUV2).xyz;
	const float3 normal = normalize(bumpMap);

	float4 grassColor = macroDetail;
	float4 slopeColor = float4(LunarSurfaceTex2.Sample(SamplerRepeatMaxAniso, texUV).xyz, 1);
	float4 rockColor = float4(LunarSurfaceTex1.Sample(SamplerRepeatMaxAniso, texUV).xyz, 1);
	float4 sandColor = float4(0.76, 0.698, 0.502, 1);

	float slope = 1.0f - normal.y;
	if (slope < 0.7)
	{
		float blendAmount = slope / 0.7f;
		macroDetail = lerp(grassColor, slopeColor, blendAmount);
	}

	if ((slope < 0.9) && (slope >= 0.7f))
	{
		float blendAmount = (slope - 0.7f) * (1.0f / (0.9f - 0.7f));
		macroDetail = lerp(slopeColor, rockColor, blendAmount);
	}

	if (slope >= 0.9)
	{
		macroDetail = rockColor;
	}

	/*if (slope < 0.6)
	{
		float blendAmount = slope / 0.6f;
		macroDetail = lerp(slopeColor, sandColor, blendAmount);
	}*/

	return calculateLambertModelLight(normal, macroDetail);

	//return float4(macroDetail, 1);
}
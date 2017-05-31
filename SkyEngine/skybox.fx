TextureCube shaderTexture:register(t0);
SamplerState SampleType: register(s0);

cbuffer ConstantBuffer: register(b0)
{
	matrix WVP;
	matrix worldMatrix;
}

struct SKYMAP_VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

SKYMAP_VS_OUTPUT mainVS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

	return output;
}

float4 mainPS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.texCoord);

	return textureColor;
}
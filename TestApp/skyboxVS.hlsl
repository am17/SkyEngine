cbuffer ConstantBuffer: register(b0)
{
	matrix WVP;
	matrix worldMatrix;
}

struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

SKYMAP_VS_OUTPUT main(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

	return output;
}
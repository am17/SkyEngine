Texture2D ObjTexture: register(t0);

SamplerState ObjSamplerState: register(s0);

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 test(VS_OUTPUT input) : SV_TARGET
{
	return float4(0.5,1,0.5,1);
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	return diffuse;
}
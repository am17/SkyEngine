TextureCube shaderTexture:register(t0);
SamplerState SampleType: register(s0);

struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

float4 main(SKYMAP_VS_OUTPUT input) : SV_Target
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.texCoord);

	return textureColor;
}
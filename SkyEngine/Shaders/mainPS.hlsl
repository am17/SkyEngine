struct PSIn_Quad
{
	float4 position     : SV_Position;
	float2 texcoord     : TEXCOORD0;
};

Texture2D g_MainTexture : register(t0);
SamplerState SamplerLinearWrap:register(s0);

cbuffer cb: register(b0)
{
	float g_MainBufferSizeMultiplier;
	float3 unused;
}

float4 MainToBackBufferPS(PSIn_Quad input) : SV_Target
{
	float4 color;
	color.rgb = g_MainTexture.SampleLevel(SamplerLinearWrap, float2((input.texcoord.x - 0.5) / g_MainBufferSizeMultiplier + 0.5f, (input.texcoord.y - 0.5) / g_MainBufferSizeMultiplier + 0.5f), 0).rgb;
	color.a = 0;
	return color;
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
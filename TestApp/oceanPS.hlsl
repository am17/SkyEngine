#define PATCH_BLEND_BEGIN		800
#define PATCH_BLEND_END			20000

Texture2D	g_texPerlin			: register(t0); // FFT wave gradient map in PS
Texture1D	g_texFresnel		: register(t1); // Fresnel factor lookup table
TextureCube	g_texReflectCube	: register(t2); // A small skybox cube texture for reflection
Texture2D	g_texGradient		: register(t3); // Perlin wave displacement & gradient map in both VS & PS
Texture2D   depthMap            : register(t4);

// Perlin noise for composing distant waves, W for height field, XY for gradient
SamplerState g_samplerPerlin	: register(s0);

// Fresnel factor lookup table
SamplerState g_samplerFresnel	: register(s1);

// A small sky cubemap for reflection
SamplerState g_samplerCube		: register(s2);

// FFT wave gradient map, converted to normal value in PS
SamplerState g_samplerGradient	: register(s3);

cbuffer cbChangePerCall : register(b0)
{
	float4x4	g_matLocal;
	float4x4	g_matWorldViewProj;
	float2		g_PerlinMovement;
	float	 g_tileSize;
	float unuse1;
}

cbuffer cbPerFrame: register(b1)
{
	float3 g_TextureWorldOffset;
	float unuse2;
	float3 cameraPosition;
	float unuse3;
}

struct MeshVertex
{
	float4 vPosition : SV_Position;
	float2 vWorldXZ  : TEXCOORD1;
	float3 LocalPos	 : TEXCOORD2;
	float3 viewDirection : TEXCOORD3;
	float4 depthPosition : TEXCOORD4;
};

float2 worldXZtoHeightUV(float2 worldXZ)
{
	// [-8,8] -> [0,1]  TBD: Ought to depend on world size though.
	return worldXZ / 16 + 0.5;
}

float calculate_linear_depth(float value)
{
	return 20000.f / (20000.f - value * (20000.f - 1.0f));
}

float4 main(MeshVertex In) : SV_TARGET
{
	float g_PerlinSize = 1.0f;
	float3 g_PerlinOctave = float3(1.12f, 0.59f, 0.23f);
	float3 g_PerlinGradient = float3(1.4f, 1.6f, 2.2f);
	float g_TexelLength_x2 = 7.81250000;

	const float4 c2 = 2.2 * float4(0.03, 0.15, 0.125, 1.0);
	const float4 c1 = 1.6 * float4(0.07f, 0.15f, 0.2f, 1.0);

	float3 eye_dir = normalize(In.viewDirection);

	float dist_2d = length(eye_dir.xy);
	float blend_factor = (PATCH_BLEND_END - dist_2d) / (PATCH_BLEND_END - PATCH_BLEND_BEGIN);
	blend_factor = clamp(blend_factor * blend_factor * blend_factor, 0, 1); 

	float2 texUV = In.vWorldXZ + 2 * float2(g_TextureWorldOffset.x, -g_TextureWorldOffset.z);

	float2 fft_tc = (blend_factor > 0) ? texUV : 0;
	float2 grad = g_texGradient.Sample(g_samplerGradient, fft_tc).xy;

	float2 perlin_tc = texUV * g_PerlinSize;
	float2 perlin_tc0 = (blend_factor < 1) ? perlin_tc * g_PerlinOctave.x + g_PerlinMovement : 0;
	float2 perlin_tc1 = (blend_factor < 1) ? perlin_tc * g_PerlinOctave.y + g_PerlinMovement : 0;
	float2 perlin_tc2 = (blend_factor < 1) ? perlin_tc * g_PerlinOctave.z + g_PerlinMovement : 0;

	float2 perlin_0 = g_texPerlin.Sample(g_samplerPerlin, perlin_tc0).xy;
	float2 perlin_1 = g_texPerlin.Sample(g_samplerPerlin, perlin_tc1).xy;
	float2 perlin_2 = g_texPerlin.Sample(g_samplerPerlin, perlin_tc2).xy;

	float2 perlin = (perlin_0 * g_PerlinGradient.x + perlin_1 * g_PerlinGradient.y + perlin_2 * g_PerlinGradient.z);

	grad = lerp(perlin, grad, blend_factor);
	float3 normal = normalize(float3(grad, g_TexelLength_x2)).xzy;

	float fresnel = clamp(abs(dot(eye_dir, normal)), 0.1, 0.9);

	float3 reflect_vec = reflect(-eye_dir, normal);
	float cos_angle = dot(normal, eye_dir);

	/*float3 g_BendParam = float3(0.1f, -0.4f, 0.2f);
	float2 ramp = float2(fresnel, fresnel);
	if (reflect_vec.z < g_BendParam.x)
	{
		ramp = lerp(ramp, g_BendParam.z, (g_BendParam.x - reflect_vec.z) / (g_BendParam.x - g_BendParam.y));
	}*/
	reflect_vec.z = max(0, reflect_vec.z);

	float4 reflection = g_texReflectCube.Sample(g_samplerCube, reflect_vec);
	reflection = reflection * reflection * 2.5f;

	//fresnel = ramp.y;

	float4 reflected_color = lerp(float4(0.38f, 0.45f, 0.56f, 1.0), reflection, fresnel);

	float4 water_color = lerp(c1, c2, abs(dot(eye_dir, normal))) * fresnel + (1.0 - fresnel) * reflected_color;

	float4 proj_tc = In.depthPosition;
	float fOwnDepth = calculate_linear_depth(proj_tc.z);

	float fSampledDepth = depthMap.Sample(g_samplerGradient, proj_tc.xy).x;

	fSampledDepth = calculate_linear_depth(fSampledDepth);

	// Для вычисления видимой толщи воды, достаточно буфера глубины, используемого при создании текстуры с преломлениями. 
	//Из значения в точке буфера нужно вычесть расстояние до водной поверхности
	//float fLinearDepth = fSampledDepth - fOwnDepth;

	//float fExpDepth = 1.0 - exp(-0.5 * fLinearDepth);
	//float fExpDepthHIGH = 1.0 - exp(-0.95 * fLinearDepth);

	//float cos_spec = clamp(dot(reflect_vec, float3(-0.936016f, -0.343206f, 0.0680013f)), 0, 1);
	//float sun_spot = pow(cos_spec, 400.0f);
	//water_color += float4(1.0f, 1.0f, 0.6f, 1) * sun_spot;

	//water_color.a = fExpDepthHIGH;// In.depthPosition.z / 200;

	//water_color = depthMap.Sample(g_samplerGradient, fft_tc);
	
	return water_color;
}
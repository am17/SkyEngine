static const int CONTROL_VTX_PER_TILE_EDGE = 9;
static const float RECIP_CONTROL_VTX_PER_TILE_EDGE = 1.0 / 9;
static const float VERTICAL_SCALE = 2;// 0.65;

Texture2D g_CoarseHeightMap: register(t0);

SamplerState SamplerClampLinear: register(s0);

cbuffer MatrixBuffer: register(b0)
{
	float4x4 g_WorldViewProj;
	float3   g_TextureWorldOffset;
	float	 g_tileSize;
};

struct AppVertex
{
	float2 position  : POSITION_2D;
	uint VertexId    : SV_VertexID;
	uint InstanceId  : SV_InstanceID;
};

struct MeshVertex
{
	float4 vPosition        : SV_Position;
	float2 vWorldXZ         : TEXCOORD0;
	float4 depthPosition : TEXTURE1;
};

float3 ReconstructPosition(AppVertex input)
{
	float iv = floor(input.VertexId * RECIP_CONTROL_VTX_PER_TILE_EDGE);
	float iu = input.VertexId - iv * CONTROL_VTX_PER_TILE_EDGE;
	float u = iu / (CONTROL_VTX_PER_TILE_EDGE - 1.0);
	float v = iv / (CONTROL_VTX_PER_TILE_EDGE - 1.0);

	float size = g_tileSize;

	return float3(u * size + input.position.x, 0, v * size + input.position.y);
}

float2 worldXZtoHeightUV(float2 worldXZ)
{
	// [-8,8] -> [0,1]  TBD: Ought to depend on world size though.
	return worldXZ / 16 + 0.5;
}

float SampleHeightForVS(Texture2D coarseTex, SamplerState coarseSampler, float2 worldXZ)
{
	const int mipLevel = 0;

	float2 texUV = worldXZtoHeightUV(worldXZ + 2 * float2(g_TextureWorldOffset.x, -g_TextureWorldOffset.z));

	if (texUV.x > 1 || texUV.y > 1 || texUV.x < 0 || texUV.y < 0)
	{
		return 0;// -10000;
	}

	float coarse = coarseTex.SampleLevel(coarseSampler, texUV, mipLevel).r;

	float result = VERTICAL_SCALE * (coarse);

	return result;
}

MeshVertex main(AppVertex input)
{
	MeshVertex output = (MeshVertex)0;

	float3 displacedPos = ReconstructPosition(input);

	float z = SampleHeightForVS(g_CoarseHeightMap, SamplerClampLinear, displacedPos.xz);

	displacedPos.y = z;

	output.vPosition = mul(float4(displacedPos, 1.0f), g_WorldViewProj);
	output.vWorldXZ = displacedPos.xz;

	output.depthPosition = output.vPosition;

	return output;
}
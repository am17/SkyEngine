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

struct AppVertex
{
	float2 position  : POSITION_2D;
	uint VertexId    : SV_VertexID;
	uint InstanceId  : SV_InstanceID;
};

struct MeshVertex
{
	float4 vPosition : SV_Position;
	float2 vWorldXZ  : TEXCOORD1;
	float3 LocalPos	 : TEXCOORD2;
	float3 viewDirection : TEXCOORD3;
	float4 depthPosition : DEPTH;
};

float3 ReconstructPosition(AppVertex input)
{
	const int CONTROL_VTX_PER_TILE_EDGE = 9;
	const float RECIP_CONTROL_VTX_PER_TILE_EDGE = 1.0 / 9;

	float iv = floor(input.VertexId * RECIP_CONTROL_VTX_PER_TILE_EDGE);
	float iu = input.VertexId - iv * CONTROL_VTX_PER_TILE_EDGE;
	float u = iu / (CONTROL_VTX_PER_TILE_EDGE - 1.0);
	float v = iv / (CONTROL_VTX_PER_TILE_EDGE - 1.0);

	float size = g_tileSize;

	return float3(u * size + input.position.x, 0.02, v * size + input.position.y);
}

MeshVertex main(AppVertex input)
{
	MeshVertex output = (MeshVertex)0;

	float3 displacedPos = ReconstructPosition(input);

	output.vPosition = mul(float4(displacedPos, 1.0f), g_matWorldViewProj);
	output.vWorldXZ = displacedPos.xz;

	float4 pos_local = mul(float4(displacedPos, 1.0f), g_matLocal);

	output.LocalPos = pos_local.xyz;

	output.viewDirection = cameraPosition.xyz - pos_local.xyz;

	output.viewDirection = normalize(output.viewDirection);

	output.depthPosition = output.vPosition;

	return output;
}
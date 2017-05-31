struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

struct AppVertex
{
	float4 position  : POSITION;
	uint VertexId    : SV_VertexID;
	uint InstanceId  : SV_InstanceID;
};

VS_OUTPUT main(AppVertex input)
{
	VS_OUTPUT output;

	output.Pos = input.position;

	output.TexCoord = float2(0, 0);

	switch (input.VertexId)
	{ 
	case 0:
		output.TexCoord = float2(0,1);
		break;
	case 1:
		output.TexCoord = float2(0, 0);
		break;
	case 2:
		output.TexCoord = float2(1, 0);
		break;
	case 3:
		output.TexCoord = float2(1, 1);
		break;
	default:
		break;
	}

	return output;
}
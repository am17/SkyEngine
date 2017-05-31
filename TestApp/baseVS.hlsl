cbuffer MatrixBuffer: register(b0)
{
	matrix WVP;
	matrix worldMatrix;
};

cbuffer CameraBuffer: register(b1)
{
	float3 cameraPosition;
	float padding;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
    //float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	
	//input.position.x += input.instancePosition.x;
    //input.position.y += input.instancePosition.y;
    //input.position.z += input.instancePosition.z;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, WVP);
	
	//output.position = input.position;

	// Store the input color for the pixel shader to use.
	//output.color = input.color;
	output.tex = input.tex;
	
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    output.normal = normalize(output.normal);

	float4 worldPosition = mul(input.position, worldMatrix);

	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	return output;
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}
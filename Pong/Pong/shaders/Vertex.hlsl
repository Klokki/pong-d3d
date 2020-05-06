cbuffer constantBuffer : register(b0)
{
	float4x4 move;
	float4x4 projection;
	float3 color;
}

struct VS_INPUT
{
	float3 inPos : POSITION;
	float3 inColor : COLOR;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float3 outColor : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	// set position
	output.outPosition = mul(float4(input.inPos, 1.0f), mul(move, projection));

	// set color
	output.outColor = input.inColor * color;

	return output;
}
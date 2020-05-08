cbuffer constantBuffer : register(b0)
{
	float4x4 move;
	float4x4 projection;
	float3 color;
}

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float3 outColor : COLOR;
};

VS_OUTPUT main(float3 inPos : POSITION, float3 inColor : COLOR)
{
	VS_OUTPUT output;

	output.outPosition = mul(float4(inPos, 1.0f), mul(move, projection));
	output.outColor = inColor * color;

	return output;
}
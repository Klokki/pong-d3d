float4 main(float4 inPosition : SV_POSITION, float3 inColor : COLOR) : SV_TARGET
{
	return float4(inColor, 1.0f);
}
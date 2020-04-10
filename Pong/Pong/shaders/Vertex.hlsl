cbuffer positionCBuffer : register(b0)
{
    float4x4 model;
    float4x4 projection;
}

struct VS_INPUT
{
    float2 inPos : POSITION;
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

    output.outPosition = mul(float4(input.inPos, 0.0f, 1.0f), mul(model, projection));
    output.outColor = input.inColor;
    return output;
}
cbuffer Transform : register(b0)
{
    float4x4 WorldViewProj;
}

struct VSInput
{
    float2 pos : POSITION; 
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

PSInput main(VSInput input)
{
    PSInput output;

    float4 worldPos = float4(input.pos, 0.0f, 1.0f);

    output.pos = mul(WorldViewProj, worldPos);

    output.uv = input.uv;

    return output;
}
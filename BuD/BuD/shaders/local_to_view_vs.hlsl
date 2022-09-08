cbuffer mvp : register(b0)
{
    matrix model;
    matrix view;
}

struct VSInput
{
    float3 pos : POSITION;
    float2 uv : PARAMS;
};

struct HSInput
{
    float4 pos : SV_POSITION;
    float2 uv : PARAMS;
};

HSInput main(VSInput i)
{
    HSInput o;
    o.pos = mul(view, mul(model, float4(i.pos, 1.0f)));
    o.uv = i.uv;
    
    return o;
}
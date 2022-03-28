cbuffer colors : register(b0)
{
    float3 color;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    float polygon : POLYGON_FLAG;
};

float4 main(GSOutput i) : SV_TARGET
{
    return i.polygon ? float4(1.0f, 0.0f, 0.0f, 1.0f) : float4(color, 1.0f);
}
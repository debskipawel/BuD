struct VSInput
{
    float3 pos0 : POSITION0;
    float3 pos1 : POSITION1;
    float3 pos2 : POSITION2;
    float3 pos3 : POSITION3;
};

struct GSInput
{
    float4 pos0 : SV_POSITION;
    float4 pos1 : POSITION1;
    float4 pos2 : POSITION2;
    float4 pos3 : POSITION3;
};

GSInput main(VSInput i)
{
    GSInput o = (GSInput) 0;
    
    o.pos0 = float4(i.pos0, 1.0);
    o.pos1 = float4(i.pos1, 1.0);
    o.pos2 = float4(i.pos2, 1.0);
    o.pos3 = float4(i.pos3, 1.0);
    
    return o;
}
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PSInput main( float2 pos : POSITION )
{
    PSInput res = (PSInput) 0;
    
    res.pos = float4(pos, 0.0, 1.0);
    res.tex = (pos + float2(1.0, 1.0)) / 2.0;
    res.tex.y = 1.0 - res.tex.y;
    
    return res;
}

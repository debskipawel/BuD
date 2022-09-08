#define INPUT_PATCH_SIZE 16
#define OUTPUT_PATCH_SIZE 16

struct HSInput
{
    float4 pos : SV_POSITION;
    float2 uv : PARAMS;
};

struct HSPatchOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct DSControlPoint
{
    float4 pos : SV_POSITION;
    float2 uv : PARAMS;
};

HSPatchOutput HS_Patch(InputPatch<HSInput, INPUT_PATCH_SIZE> ip, uint patchId : SV_PrimitiveID)
{
    HSPatchOutput o;

    float4 midPoints[4] =
    {
        0.5f * (ip[0].pos + ip[12].pos),
        0.5f * (ip[0].pos + ip[3].pos),
        0.5f * (ip[3].pos + ip[15].pos),
        0.5f * (ip[12].pos + ip[15].pos)
    };
    
    for (int i = 0; i < 4; i++)
    {
        o.edges[i] = -16 * log10(0.01 * midPoints[i].z) + 0;
    }
	
    float4 mid = 0.5f * (ip[5].pos + ip[10].pos);
    o.inside[0] = o.inside[1] = -16 * log10(0.01 * mid.z) + 0;
	
    return o;
}

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(OUTPUT_PATCH_SIZE)]
[patchconstantfunc("HS_Patch")]
DSControlPoint main(InputPatch<HSInput, INPUT_PATCH_SIZE> ip, uint i : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID)
{
    DSControlPoint o;
    o.pos = ip[i].pos;
    return o;
}
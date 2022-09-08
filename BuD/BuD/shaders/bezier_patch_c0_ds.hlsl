#define OUTPUT_PATCH_SIZE 16

cbuffer cbProj : register(b0)
{
    matrix projMatrix;
};

struct HSPatchOutput
{
    float edges[4] : SV_TessFactor;
    float insideFactors[2] : SV_InsideTessFactor;
};

struct DSControlPoint
{
    float4 pos : SV_POSITION;
    float2 uv : PARAMS;
};

struct PSInput
{
    float4 pos : SV_POSITION;
};

float4 DeCasteljau3(int n, float t)
{
    matrix bernsteinBasis =
    {
        { 1.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
    };

    float u = 1.0 - t;

    for (int j = 1; j <= n; j++)
    {
        bernsteinBasis[j][0] = bernsteinBasis[j - 1][0] * u;

        for (int i = 1; i <= j; i++)
        {
            bernsteinBasis[j][i] = bernsteinBasis[j - 1][i] * u + bernsteinBasis[j - 1][i - 1] * t;
        }
    }

    return float4(bernsteinBasis[n][0], bernsteinBasis[n][1], bernsteinBasis[n][2], bernsteinBasis[n][3]);
}

float4 PointOnBezier(const OutputPatch<DSControlPoint, OUTPUT_PATCH_SIZE> input, float2 uv)
{
    float4 bernsteinU = DeCasteljau3(3, uv.x);
    float4 bernsteinV = DeCasteljau3(3, uv.y);
    
    float4 result = 0.0;
    
    for (int i = 0; i < 4; i++)
    {
        float4 partialResult = 0.0;
        
        for (int j = 0; j < 4; j++)
        {
            partialResult += input[4 * i + j].pos * bernsteinU[j];
        }

        result += partialResult * bernsteinV[i];
    }

    return result;
}

[domain("quad")]
PSInput main(HSPatchOutput factors, float2 uv : SV_DomainLocation,
	const OutputPatch<DSControlPoint, OUTPUT_PATCH_SIZE> input)
{
    PSInput o;
	
    float4 viewPos = PointOnBezier(input, uv);
    
    //float2 params = float2(minU + (maxU - minU) * uv.y, minV + (maxV - minV) * uv.x);
    //float mipLevel = 6 - log2(-16 * log10(viewPos.z + 0.01));
    
    o.pos = mul(projMatrix, viewPos);
    
    return o;
}
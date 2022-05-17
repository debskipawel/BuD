cbuffer mvp : register(b0)
{
    matrix mvp;
}

cbuffer params : register(b1)
{
    uint u;
    uint v;
    uint uStart;
    uint vStart;
}

struct GSInput
{
	float4 pos0 : SV_POSITION;
	float4 pos1 : POSITION1;
	float4 pos2 : POSITION2;
    float4 pos3 : POSITION3;
};

struct GSOutput
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

[maxvertexcount(256)]
void main(
	lineadj GSInput input[4], 
	inout LineStream<GSOutput> output
)
{
    GSOutput element;
    element.pos = mul(mvp, input[0].pos1);
    
    output.Append(element);
    
    element.pos = float4(1.0, 1.0, 1.0, 1.0);
    
    output.Append(element);
    output.RestartStrip();

}
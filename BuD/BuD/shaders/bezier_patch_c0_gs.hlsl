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

float4 GetPoint(GSInput input[4], float u, float v)
{
    float4 uDeCasteljau = DeCasteljau3(3, u);
    float4 vDeCasteljau = DeCasteljau3(3, v);

    float4 result = float4(0.0, 0.0, 0.0, 0.0);

    for (int u = 0; u < 4; u++)
    {
        float4 partResult = float4(0.0, 0.0, 0.0, 0.0);

        partResult += input[u].pos0 * vDeCasteljau[0];
        partResult += input[u].pos1 * vDeCasteljau[1];
        partResult += input[u].pos2 * vDeCasteljau[2];
        partResult += input[u].pos3 * vDeCasteljau[3];

        result += partResult * uDeCasteljau[u];
    }

    return result;
}

[maxvertexcount(252)]
void main(
	lineadj GSInput input[4], 
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    
    int vertices = 0;
    
    int samplesU = u + 1;
    int samplesV = v + 1;

    int verticesCount = samplesU * samplesV;

    float minDom = 0.0;
    float maxDom = 1.0;

    float du = (maxDom - minDom) / (samplesU - 1);
    float dv = (maxDom - minDom) / (samplesV - 1);

    float u = minDom + uStart * du;
    
    int vStartMod = vStart;

    for (int i = uStart; i < samplesU - 1; i++)
    {
        float v = minDom + vStartMod * dv;

        for (int j = vStartMod; j < samplesV - 1; j++)
        {
            float4 v1 = mul(mvp, GetPoint(input, u, v));
            float4 v2 = mul(mvp, GetPoint(input, u, v + dv));
            float4 v3 = mul(mvp, GetPoint(input, u + du, v));
            float4 v4 = mul(mvp, GetPoint(input, u + du, v + dv));

            element.pos = v1;
            output.Append(element);
            element.pos = v2;
            output.Append(element);
            element.pos = v3;
            output.Append(element);
            
            output.RestartStrip();
            
            element.pos = v2;
            output.Append(element);
            element.pos = v3;
            output.Append(element);
            element.pos = v4;
            output.Append(element);
            
            output.RestartStrip();

            v += dv;
            vertices += 6;
            
            if (vertices == 252)
            {
                return;
            }
        }

        vStartMod = 0;
        u += du;
    }
}
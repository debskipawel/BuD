cbuffer parameters : register(b0)
{
    uint samples;
}

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
	lineadj float4 input[4] : SV_POSITION,
	inout LineStream<GSOutput> lineOutput
)
{
    int esamples = 16;
    float dt = 1.0 / esamples;

    for (float t = 0.0; t < 1.0; t += dt)
    {
        GSOutput value = (GSOutput) 0;

        float4 b = DeCasteljau3(3, t);
        value.pos = input[0] * b.x + input[1] * b.y + input[2] * b.z + input[3] * b.w;

        lineOutput.Append(value);
    }

    lineOutput.RestartStrip();
}

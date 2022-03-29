cbuffer parameters : register(b0)
{
	uint samples;
    int bezierPolygonVisible;
}

struct GSOutput
{
	float4 pos : SV_POSITION;
    float polygon : POLYGON_FLAG;
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

[maxvertexcount(200)]
void main(
	lineadj float4 input[4] : SV_POSITION,
	inout LineStream<GSOutput> lineOutput
)
{
    uint sampleCount = min(200, samples);
	float dt = 1.0 / sampleCount;

	for (float t = 0.0; t <= 1.0; t += dt)
	{
		GSOutput value = (GSOutput) 0;

		float4 b = DeCasteljau3(3, t);
		value.pos = input[0] * b.x + input[1] * b.y + input[2] * b.z + input[3] * b.w;

		lineOutput.Append(value);
	}

	lineOutput.RestartStrip();

    if (!bezierPolygonVisible)
    {
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        GSOutput value = (GSOutput) 0;
        value.pos = input[i];
        value.polygon = 1.0f;

        lineOutput.Append(value);
    }

    lineOutput.RestartStrip();
}

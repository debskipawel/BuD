cbuffer parameters : register(b0)
{
    uint samples;
}

cbuffer mvp : register(b1)
{
    matrix mvp;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
};

[maxvertexcount(200)]
void main(
	lineadj float4 input[4] : SV_POSITION,
	inout LineStream<GSOutput> lineOutput
)
{
    uint sampleCount = min(200, samples);
    float dt = 1.0 / sampleCount;

    float eps = 0.0001;
	
    for (float t = 0.0; t <= 1.0 + eps; t += dt)
    {
        GSOutput value = (GSOutput) 0;
        value.pos = mul(mvp, float4(input[0].xyz + t * (input[1].xyz + t * (input[2].xyz + t * input[3].xyz)), 1.0));

        lineOutput.Append(value);
    }

    lineOutput.RestartStrip();
}

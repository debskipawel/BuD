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
    float bezier : POLYGON_FLAG;
};

[maxvertexcount(200)]
void main(
	lineadj float4 input[4] : SV_POSITION,
	inout LineStream<GSOutput> lineOutput
)
{
    uint sampleCount = min(200, samples);
    float l = input[0].w;
    float dt = l / sampleCount;

    float eps = 0.0001;
	
    if (l == 0.0f)
        return;
    
    for (float t = 0.0; t <= l + eps; t += dt)
    {
        GSOutput value = (GSOutput) 0;
        value.pos = mul(mvp, float4(input[0].xyz + t * (input[1].xyz + t * (input[2].xyz + t * input[3].xyz)), 1.0));

        lineOutput.Append(value);
    }

    lineOutput.RestartStrip();
}

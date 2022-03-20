cbuffer transformations : register(b0)
{
	matrix MVP;
}

struct VSOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VSOut main(float3 pos : POSITION, float3 color : COLOR)
{
	VSOut o;
	o.pos = mul(MVP, float4(pos, 1.0));
    o.color = float4(color, 1.0);

	return o;
}
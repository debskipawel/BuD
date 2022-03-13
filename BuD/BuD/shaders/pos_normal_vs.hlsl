cbuffer transformations : register(b0)
{
	matrix MVP;
}

float4 main(float3 pos : POSITION, float3 normal: NORMAL) : SV_POSITION
{
	return mul(MVP, float4(pos, 1.0));
}

float4 main(float3 pos : POSITION, float3 normal: NORMAL) : SV_POSITION
{
	return float4(pos, 1.0);
}
cbuffer transformations : register(b0)
{
    int id;
}

float4 main() : SV_TARGET
{
    int r = id / (256 * 256);
    int g = id / 256 - r * 256;
    int b = id % 256;

    float3 res = float3(r, g, b) / 255.0f;

	return float4(res, 1.0f);
}
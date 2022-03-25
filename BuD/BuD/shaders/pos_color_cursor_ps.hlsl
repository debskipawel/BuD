struct VSOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(VSOut i) : SV_TARGET
{
    float r = i.color.x < 1.0 ? 0.0 : i.color.x;
    float g = i.color.y < 1.0 ? 0.0 : i.color.y;
    float b = i.color.z < 1.0 ? 0.0 : i.color.z;

	return float4(r, g, b, 1.0f);
}
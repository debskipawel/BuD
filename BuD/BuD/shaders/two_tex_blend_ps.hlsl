Texture2D leftEye : register(t0);
Texture2D rightEye : register(t1);
SamplerState texSampler : register(s0);

struct PSInput
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    float2 tex = input.tex;
	
    float4 left = leftEye.Sample(texSampler, tex);
    float4 right = rightEye.Sample(texSampler, tex);

    //float r = 0.299 * left.r + 0.587 * left.g + 0.114 * left.b;
    //float g = 0.299 * right.r + 0.587 * right.g + 0.114 * right.b;
    //float b = 0.299 * right.r + 0.587 * right.g + 0.114 * right.b;
    
    float r = 0.4561 * left.r + 0.500484 * left.g + 0.176381 * left.b - 0.0434706 * right.r - 0.0879388 * right.g - 0.00155529 * right.b;
    float g = -0.0400822 * left.r - 0.0378246 * left.g - 0.0157589 * left.b + 0.378476 * right.r + 0.73364 * right.g - 0.0184503 * right.b;
    float b = -0.0152161 * left.r - 0.0205971 * left.g - 0.00546856 * left.b - 0.0721527 * right.r - 0.112961 * right.g + 1.2264 * right.b;
    
	return float4(r, g, b, 1.0f);
}
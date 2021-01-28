#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float4 output = mainTexture.Sample(mainSampler, pixel.uv) + subTexture_1.Sample(mainSampler, pixel.uv);
    output.w = 1.0f;
    return output;
}
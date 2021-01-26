#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float4 output = mainTexture.Sample(mainSampler, pixel.uv);

    for (int i = 0; i < sampleCount; i++) {
        output += gausOffset[i].z * mainTexture.Sample(mainSampler, pixel.uv + gausOffset[i].xy);
    }
    //output.w = 1;
    return output;
}
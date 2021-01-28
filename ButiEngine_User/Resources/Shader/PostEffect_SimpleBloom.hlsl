#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float4 output = 0;

    for (int i = 0; i < 15;i++) {
        output += gausOffset[i].z * mainTexture.Sample(mainSampler, pixel.uv + gausOffset[i].xy  );
    }
    output.w = 1;
    return output;
}
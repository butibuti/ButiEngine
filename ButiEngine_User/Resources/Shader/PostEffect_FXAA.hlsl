
#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    return float4(FxaaPixelShader(pixel.uv, mainTexture, pixelScale),1.0);
}
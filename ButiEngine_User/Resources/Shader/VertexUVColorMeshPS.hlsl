#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV_Color pixel) : SV_TARGET
{
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba);
	return Tex * pixel.color;
}

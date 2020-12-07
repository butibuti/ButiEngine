#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV_Fog pixel) : SV_TARGET
{
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba);
	float3 retRGB = lerp(fogColor.rgb, (Tex ).rgb, pixel.fog);
	return float4(retRGB, Tex.a);
}

#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV_Normal_Color pixel) : SV_TARGET
{
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);
	Light.a = 0;

	float4 Color = pixel.color;
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba);
	return  Color+Light;
}

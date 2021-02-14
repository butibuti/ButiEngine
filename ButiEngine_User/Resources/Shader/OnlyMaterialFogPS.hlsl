#include"DefaultShader.hlsli"
float4 PSMain(Pixel_UV_Normal_Fog pixel) : SV_TARGET
{ 
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);

	Light.rgb += specular.rgb * specular.a;
	float4 retRGBA = lerp(fogColor, (Light), pixel.fog);
	retRGBA.a=1.0;
	return  retRGBA;//,0.5f);
}
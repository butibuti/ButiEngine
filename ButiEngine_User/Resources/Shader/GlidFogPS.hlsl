#include"DefaultShader.hlsli"


float4 PSMain(Pixel_UV_Fog pixel) : SV_TARGET
{

    float divisions = 1*modelMatrix[0][0];
    float thickness = 0.8/modelMatrix[0][0];
	float4 Color = float4(0.1,0.1,0.1,1);
	pixel.uv*=divisions;
   
    pixel.uv.x = frac(pixel.uv.x );
    pixel.uv.x =min(pixel.uv.x, 1.0 - pixel.uv.x);
    float delta = fwidth(pixel.uv.x);


	float glidX= smoothstep(pixel.uv.x- delta, pixel.uv.x+ delta,thickness);

    pixel.uv.y = frac(pixel.uv.y);
    pixel.uv.y = min(pixel.uv.y, 1.0 - pixel.uv.y);
    delta = fwidth(pixel.uv.y);


	float glidY= smoothstep(pixel.uv.y- delta, pixel.uv.y+ delta,thickness);




	Color+=float4(0.5f,0.5f,0.5f,1.0f)*clamp( glidX+glidY,0.0f,1.0f);

    float3 retRGB = lerp(fogColor.rgb, (Color).rgb, pixel.fog);
    return float4(retRGB, Color.a);//,0.5f);
}
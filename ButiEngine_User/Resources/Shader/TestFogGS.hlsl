#include"DefaultShader.hlsli"


[maxvertexcount(3)]
void GSMain(
    triangle Vertex_UV_Normal input[3],
    inout TriangleStream<Pixel_UV_Normal_Fog> stream)
{
    float3 push = input[0].normal + input[1].normal + input[2].normal;
    for (int i = 0; i < 3; i++) {
        Pixel_UV_Normal_Fog pixel;
        float per =pushPower.w;
        pixel.position = input[i].position+float4(push *per+float3(0,(-per*9.8*9.8*0.1),0) , 0);

        pixel.position.y = max(pixel.position.y, bottom);
        float dist = length(pixel.position - cameraPos);
        pixel.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);

        pixel.position = mul(pixel.position, viewMatrix);
        pixel.position = mul(pixel.position, projectionMatrix);
        pixel.normal = input[i].normal;
        pixel.uv = input[i].uv;


        stream.Append(pixel);
    }
    stream.RestartStrip();
}
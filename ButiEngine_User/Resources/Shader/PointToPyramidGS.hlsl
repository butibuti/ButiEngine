#include"DefaultShader.hlsli"

static const float4 array_Offset[6]={
    float4(0.5f,0.5f,0.0,0),
    float4(0.5f,-0.5f,0.0,0),
    float4(-0.5f,-0.5f,0.0,0),

    float4(0.5f,0.5f,0.0,0),
    float4(-0.5f,-0.5f,0.0,0),
    float4(-0.5f,0.5f,0.0,0),
};
static const float2 array_UV[6]={
    float2(1.0,0.0),
    float2(1.0,1.0),
    float2(0.0,1.0),
    float2(1.0,0.0),
    float2(0.0,1.0),
    float2(0.0,0.0),
};

[maxvertexcount(6)]
void GSMain(
    point Vertex_UV_Normal input[1],
    inout TriangleStream<Pixel_UV_Normal> stream)
{

    float4 position = input[0].position;

    position = mul(position, viewMatrix);

    Pixel_UV_Normal pixel;

    pixel.normal = input[0].normal;
    float scale = 0.1f;
    for (uint i = 0; i < 6; i+=3) {

    pixel.position = position + array_Offset[i] * scale;
    pixel.position = mul(pixel.position, projectionMatrix);
    pixel.uv = array_UV[i];
    stream.Append(pixel);
    pixel.position = position + array_Offset[i+1] * scale;
    pixel.position = mul(pixel.position, projectionMatrix);
    pixel.uv = array_UV[i+1];
    stream.Append(pixel);
    pixel.position = position + array_Offset[i+2] * scale;
    pixel.position = mul(pixel.position, projectionMatrix);
    pixel.uv = array_UV[i+2];
    stream.Append(pixel);

    stream.RestartStrip();
    }



}
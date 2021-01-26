#include"DefaultShader.hlsli"

static const float3 array_Offset[36]={
    float3(0.5f,0.5f,-0.5),
    float3(0.5f,-0.5f,-0.5),
    float3(-0.5f,-0.5f,-0.5),

    float3(0.5f,0.5f,-0.5),
    float3(-0.5f,-0.5f,-0.5),
    float3(-0.5f,0.5f,-0.5),

    
    float3(-0.5f,-0.5f,0.5),
    float3(0.5f,-0.5f,0.5),
    float3(0.5f,0.5f,0.5),

    
    float3(-0.5f,0.5f,0.5),
    float3(-0.5f,-0.5f,0.5),
    float3(0.5f,0.5f,0.5),

    float3(0.5f,0.5f,0.5),
    float3(0.5f,0.5f,-0.5),
    float3(-0.5f,0.5f,-0.5),

    float3(0.5f,0.5f,0.5),
    float3(-0.5f,0.5f,-0.5),
    float3(-0.5f,0.5f,0.5),

    float3(-0.5f,-0.5f,-0.5),
    float3(0.5f,-0.5f,-0.5),
    float3(0.5f,-0.5f,0.5),

    
    float3(-0.5f,-0.5f,0.5),
    float3(-0.5f,-0.5f,-0.5),
    float3(0.5f,-0.5f,0.5),

    float3(0.5f,-0.5f,0.5),
    float3(0.5f,-0.5f,-0.5),
    float3(0.5f,0.5f,-0.5),

    float3(0.5f,0.5f,0.5),
    float3(0.5f,-0.5f,0.5),
    float3(0.5f,0.5f,-0.5),

    float3(-0.5f,0.5f,-0.5),
    float3(-0.5f,-0.5f,-0.5),
    float3(-0.5f,-0.5f,0.5),
    
    float3(-0.5f,0.5f,-0.5),
    float3(-0.5f,-0.5f,0.5),
    float3(-0.5f,0.5f,0.5),
};
static const float2 array_UV[6]={
    float2(1.0,0.0),
    float2(1.0,1.0),
    float2(0.0,1.0),
    float2(1.0,0.0),
    float2(0.0,1.0),
    float2(0.0,0.0),
};

static const float3 array_normal[36] = {
    float3(0,0,-1),
    float3(0,0,-1),
    float3(0,0,-1),
    float3(0,0,-1),
    float3(0,0,-1),
    float3(0,0,-1),
    float3(0,0,1),
    float3(0,0,1),
    float3(0,0,1),
    float3(0,0,1),
    float3(0,0,1),
    float3(0,0,1),
    float3(1,0,0),
    float3(1,0,0),
    float3(1,0,0),
    float3(1,0,0),
    float3(1,0,0),
    float3(1,0,0),
    float3(-1,0,0),
    float3(-1,0,0),
    float3(-1,0,0),
    float3(-1,0,0),
    float3(-1,0,0),
    float3(-1,0,0),
    float3(0,1,0),
    float3(0,1,0),
    float3(0,1,0),
    float3(0,1,0),
    float3(0,1,0),
    float3(0,1,0),
    float3(0,-1,0),
    float3(0,-1,0),
    float3(0,-1,0),
    float3(0,-1,0),
    float3(0,-1,0),
    float3(0,-1,0),
};

float3x3 QuaternionToMatrix(float4 axis) {
    float3 a = normalize(axis.xyz);
    axis.w *= 3.14159265;
    float s = sin(axis.w);
    float c = cos(axis.w);
    float r = 1.0 - c;
    return float3x3(
        a.x * a.x * r + c, a.y * a.x * r + a.z * s, a.z * a.x * r - a.y * s,
        a.x * a.y * r - a.z * s, a.y * a.y * r + c, a.z * a.y * r + a.x * s,
        a.x * a.z * r + a.y * s, a.y * a.z * r - a.x * s, a.z * a.z * r + c
        );
}

[maxvertexcount(36)]
void GSMain(
    point  Vertex_UV_Normal_Color  input[1],
    inout TriangleStream<Pixel_UV_Normal_Color> stream)
{

    float4 position = input[0].position;

    float4x4 vp = mul(viewMatrix, projectionMatrix);


    Pixel_UV_Normal_Color pixel;

    pixel.normal = input[0].normal;
    pixel.color = float4(input[0].normal, 1);
    float3x3 rotation = QuaternionToMatrix(input[0].color);
    float scale = 0.1f*input[0].uv.x;



    for (uint i = 0; i < 36; i+=6) {

        pixel.position = position + float4(mul(array_Offset[i] * scale, rotation), 0);
        pixel.position = mul(pixel.position, vp);
        pixel.uv = array_UV[0];
        pixel.normal = mul(array_normal[i], rotation);
        stream.Append(pixel);
        pixel.position = position + float4(mul(array_Offset[i + 1] * scale, rotation), 0);
        pixel.position = mul(pixel.position, vp);
        pixel.uv = array_UV[1]; 
        pixel.normal = mul(array_normal[i+1], rotation);
        stream.Append(pixel);
        pixel.position = position + float4(mul(array_Offset[i + 2] * scale, rotation), 0);
        pixel.position = mul(pixel.position, vp);
        pixel.uv = array_UV[2];
        pixel.normal = mul(array_normal[i + 2], rotation);
        stream.Append(pixel);

        stream.RestartStrip();


        pixel.position = position + float4(mul(array_Offset[i+3] * scale, rotation), 0);
        pixel.position = mul(pixel.position, vp);
        pixel.uv = array_UV[3];
        pixel.normal = mul(array_normal[i + 3], rotation);
        stream.Append(pixel);
        pixel.position = position + float4(mul(array_Offset[i + 4] * scale, rotation), 0);
        pixel.position = mul(pixel.position, vp);
        pixel.uv = array_UV[4]; 
        pixel.normal = mul(array_normal[i + 4], rotation);
        stream.Append(pixel);
        pixel.position = position + float4(mul(array_Offset[i + 5] * scale, rotation), 0);
        pixel.position = mul(pixel.position, vp);
        pixel.uv = array_UV[5];
        pixel.normal = mul(array_normal[i + 5], rotation);
        stream.Append(pixel);

    stream.RestartStrip();
    }



}
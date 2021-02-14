#include"DefaultShader.hlsli"

[maxvertexcount(3)]
void GSMain(
    triangle Vertex_UV_Normal input[3],
    inout TriangleStream<Pixel> stream)
{
        
        for (int j = 0; j<3; j++) {
            Pixel outlinePixel;
            float4 viewPosition= mul(input[j].position, viewMatrix);
            outlinePixel.position = input[j].position + float4(input[j].normal, 0) * lightDir.w*(viewPosition.z/viewPosition.w/5);

            outlinePixel.position = mul(outlinePixel.position, viewMatrix);
            outlinePixel.position = mul(outlinePixel.position, projectionMatrix);
            stream.Append(outlinePixel);
        }
}
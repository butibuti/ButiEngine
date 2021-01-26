#include"DefaultShader.hlsli"

Pixel_UV_Normal VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal =  mul(vertex.normal, (float3x3)modelMatrix);

	output.uv = vertex.uv;
	return output;
}
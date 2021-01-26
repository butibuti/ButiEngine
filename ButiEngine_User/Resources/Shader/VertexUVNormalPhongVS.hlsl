#include"DefaultShader.hlsli"

Pixel_UV_Normal_Phong VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_Phong output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal = mul(vertex.normal, (float3x3)modelMatrix);

	output.uv = vertex.uv;
	output.modelPosition = mul(vertex.position, modelMatrix);
	return output;
}
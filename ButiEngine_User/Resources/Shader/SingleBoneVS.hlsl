
#include"DefaultShader.hlsli"


Pixel_UV_Normal VSMain(Vertex_UV_Normal_SingleBone vertex)
{
	Pixel_UV_Normal output;
	matrix bm = bones[vertex.bone];
	output.position = mul(bm, vertex.position);
	output.position = mul(output.position, modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = mul(vertex.normal, (float3x3)bm);
	output.normal = mul(output.normal, (float3x3)modelMatrix);
	output.uv = vertex.uv;
	return output;
}
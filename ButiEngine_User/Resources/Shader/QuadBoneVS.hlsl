#include"DefaultShader.hlsli"

Pixel_UV_Normal VSMain(Vertex_UV_Normal_QuadBone vertex)
{
	Pixel_UV_Normal output;
	matrix bm = vertex.weight_01 * bones[vertex.boneIndex_1] + vertex.weight_02 * bones[vertex.boneIndex_2] + vertex.weight_03 * bones[vertex.boneIndex_3] + vertex.weight_04 * bones[vertex.boneIndex_4];

	output.position = mul(bm, vertex.position);

	output.position = mul(output.position, modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = mul(vertex.normal, (float3x3)bm);
	output.normal = mul(output.normal, (float3x3)modelMatrix);
	output.uv = vertex.uv;
	return output;
}
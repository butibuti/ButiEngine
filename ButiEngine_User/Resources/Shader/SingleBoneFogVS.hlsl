
#include"DefaultShader.hlsli"


Pixel_UV_Normal_Fog VSMain(Vertex_UV_Normal_SingleBone vertex)
{
	Pixel_UV_Normal_Fog output;
	matrix bm = bones[vertex.bone];
	output.position = mul(bm, vertex.position);
	output.position = mul(output.position, mvpMatrix);
	float dist = length(mul(vertex.position, modelMatrix) - cameraPos);
	output.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);
	output.normal = mul(vertex.normal, (float3x3)bm);
	output.normal = mul(output.normal, (float3x3)modelMatrix);
	output.uv = vertex.uv;
	return output;
}
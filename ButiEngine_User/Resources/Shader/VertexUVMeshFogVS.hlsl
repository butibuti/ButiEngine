#include"DefaultShader.hlsli"

Pixel_UV_Fog VSMain(Vertex_UV vertex)
{
	Pixel_UV_Fog output;
	output.position = mul(vertex.position, mvpMatrix);
	float dist = length(mul(vertex.position, modelMatrix) - cameraPos);
	output.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);
	output.uv = vertex.uv;
	return output;
}

#include"DefaultShader.hlsli"

Pixel_UV_Normal_Color_Fog VSMain(Vertex_UV_Normal_Color vertex)
{
	Pixel_UV_Normal_Color_Fog output;
	output.position = mul(vertex.position, mvpMatrix);
	float dist = length(mul(vertex.position, modelMatrix) - cameraPos);
	output.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);
	output.uv = vertex.uv;
	output.normal = mul(vertex.normal, (float3x3)modelMatrix);
	output.color = vertex.color;
	output.color.a = 0;
	return output;
}

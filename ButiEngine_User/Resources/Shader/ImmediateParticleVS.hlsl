#include"DefaultShader.hlsli"

Vertex_UV_Normal_Color VSMain(Vertex_UV_Normal_Color vertex)
{
	vertex.position = mul(vertex.position, modelMatrix);
	return vertex;
}
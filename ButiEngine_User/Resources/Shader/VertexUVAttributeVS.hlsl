#include"DefaultShader.hlsli"

Vertex_UV VSMain(Vertex_UV vertex)
{
	vertex.position = mul(vertex.position, modelMatrix);
	return vertex;
}

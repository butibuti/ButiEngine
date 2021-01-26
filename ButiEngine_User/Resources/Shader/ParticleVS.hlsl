#include"DefaultShader.hlsli"


uint mod(uint x, uint y)
{
	return x - y * (x / y);
}

Vertex_UV_Color VSMain(Vertex_UV_Normal vertex)
{
	uint pushNoise =mod( vertex.uv.x * 128 + vertex.uv.y * 256,MaxRange+ noise);
	float fource = time * power+pushNoise+MinRange;

	fource = fmod(fource,MaxRange);

	Vertex_UV_Color output;

	output.position = vertex.position + float4(vertex.normal * fource, 0);
	output.position = mul(output.position, modelMatrix);

	fource= smoothstep(MinRange, MaxRange, fource);
	output.color = (startColor * (1 -fource) + endColor * fource);
	output.uv =float2( (1 - fource*fource)*size+minSize,0);
	return output;
}
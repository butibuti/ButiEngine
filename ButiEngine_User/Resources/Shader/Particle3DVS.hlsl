#include"DefaultShader.hlsli"


uint mod(uint x, uint y)
{
	return x - y * (x / y);
}


Vertex_UV_Normal_Color VSMain(Vertex_UV_Normal vertex)
{

	Vertex_UV_Normal_Color output;
	output.uv = float2(0, 0);
	uint pushNoise = mod(vertex.uv.x * 128 + vertex.uv.y * 256, MaxRange + noise);
	float fource = time * power + pushNoise + MinRange;

	//fource = fmod(fource, MaxRange);

	output.position = vertex.position + float4(vertex.normal * fource, 0);
	output.position = mul(output.position, modelMatrix);

	fource = smoothstep(MinRange, MaxRange, fource);
	output.normal = (startColor * (1 - fource) + endColor * fource).xyz;
	output.uv.x =minSize+ (1 - fource * fource)* size;

	output.color = float4(vertex.uv.x,vertex.uv.y,fource,0.1+fource*PI*rotationPase);

	return output;
}
#include"DefaultShader.hlsli"

cbuffer OutLineProp : register(b1)
{
	float4 lineColor : packoffset(c0);
	float lineWidth	: packoffset(c1);
};
float4 PSMain(Pixel pixel):SV_TARGET
{
	return lineColor;
	
}
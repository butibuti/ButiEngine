
#include"DefaultShader.hlsli"

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float4 Tex=0;
	float nx =120;
    float ny = nx/pixelScale.x/pixelScale.y;

	float2 samplePos= pixel.uv;
	samplePos.x = floor(samplePos.x * nx) / nx;
	samplePos.y = floor(samplePos.y * ny) / ny;


	for (float x = 0.0; x < 16.0; x += 1.0) {
		for (float y = 0.0; y < 16.0; y += 1.0) {
			Tex += mainTexture.Sample(mainSampler, (samplePos + float2(x* pixelScale.x, y* pixelScale.y)) );
		}
	}
	return Tex * 0.00390625;
}


#include"DefaultShader.hlsli"
#define Time			lightDir.x
#define VertGlitchPase	lightDir.y
#define HorzGlitchPase	lightDir.z
#define GlitchStepValue	lightDir.w
#define Distortion		0.05

float4 PSMain(Pixel_UV pixel) : SV_TARGET
{



	float vertNoise = WhiteNoise(float2(floor((pixel.uv.x) / VertGlitchPase) * VertGlitchPase, Time *0.1));
	float horzNoise = WhiteNoise(float2(floor((pixel.uv.y) / HorzGlitchPase) * HorzGlitchPase, Time*0.2));

	float vertGlitchStrength = vertNoise / GlitchStepValue;
	float horzGlitchStrength = horzNoise / GlitchStepValue;

	vertGlitchStrength = vertGlitchStrength * 2.0 - 1.0;
	horzGlitchStrength = horzGlitchStrength * 2.0 - 1.0;

	float V = step(vertNoise, GlitchStepValue * 2) * vertGlitchStrength ;
	float H = step(horzNoise, GlitchStepValue) * horzGlitchStrength ;


	float2 samplePoint = pixel.uv;
	samplePoint -= float2(0.5, 0.5);
	float distPower = pow(length(samplePoint), Distortion);
	samplePoint *= float2(distPower,distPower);
	samplePoint += float2(0.5 , 0.5);
	float4 Tex = mainTexture.Sample(mainSampler, samplePoint);//float4 (RadialBlur(pixel.uv),1.0);

	float sinv = sin(samplePoint.y * 2 - Time * -0.1);
	float steped =1- step(0.99, sinv * sinv);

	Tex.rgb -= steped * abs(sin(samplePoint.y * 50.0 - Time * 1.0)) * 0.05;
	Tex.rgb -= steped * abs(sin(samplePoint.y * 100.0 + Time * 2.0)) * 0.08;

	float timeFrac = steped * step(0.8, frac(Time)) * 0.01;
	samplePoint.x += timeFrac*(V+H);
	Tex.g += mainTexture.Sample(mainSampler, samplePoint).g;
	samplePoint.x += 2 * timeFrac*(V+H);
	Tex.b += mainTexture.Sample(mainSampler, samplePoint).b;

	float vignette = length(float2(0.5,0.5) - pixel.uv);

	vignette = clamp(vignette - 0.2, 0, 1);

	//samplePoint.x += (1- steped)*( abs( sin(samplePoint.y * 200.0 - Time * 2.0)) * 0.02f* (   0.5- samplePoint.x));


	//Tex.r = mainTexture.Sample(mainSampler, samplePoint).r;//float4 (RadialBlur(pixel.uv),1.0);

	Tex.rgb +=(1-steped )* 0.1;
	float noise = vertNoise * 0.1;
	Tex.rgb += steped * float3(noise, noise, noise);
	Tex.rgb -= vignette;
	return Tex;
}

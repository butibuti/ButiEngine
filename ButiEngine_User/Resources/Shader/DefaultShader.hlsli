#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
#define PI float(3.14159265358979323846264338327950288f)
matrix inverse(matrix m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    matrix ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

cbuffer Object : register(b0)
{
	matrix modelMatrix: packoffset(c0);
	matrix viewMatrix: packoffset(c4);
    matrix projectionMatrix: packoffset(c8);
    matrix mvpMatrix: packoffset(c12);

};
cbuffer Material : register(b1)
{
	float4 emissive : packoffset(c0);
	float4 diffuse	: packoffset(c1);
	float4 ambient  : packoffset(c2);
	float4 specular : packoffset(c3);
};
cbuffer Fog:register(b2) {

    float4 fogColor:packoffset(c0);
    float4 cameraPos:packoffset(c1);
    float2 fogCoord:packoffset(c2);
    float2 pixelScale:packoffset(c2.z);
}
cbuffer Light : register(b3)
{
	float4 lightDir	: packoffset(c0);
};
cbuffer GausParameter : register(b4)
{
    float3 gausOffset[16]: packoffset(c0);
    int sampleCount : packoffset(c12);
};
cbuffer Bone : register(b4)
{
    matrix bones[256]: packoffset(c0);
};

cbuffer ExVariables : register(b5)
{
    float4 pushPower : packoffset(c0);
    float bottom : packoffset(c1);
};


cbuffer ParticleParameter : register(b4)
{
    float4 startColor: packoffset(c0);
    float4 endColor: packoffset(c1);
    float time : packoffset(c2);
    float power : packoffset(c2.y);
    uint noise: packoffset(c2.z);
    float MaxRange : packoffset(c2.w);
    float MinRange : packoffset(c3.x);
    float size : packoffset(c3.y);
    float minSize : packoffset(c3.z);
    float rotationPase : packoffset(c3.w);
};

Texture2D mainTexture : register(t0);
Texture2D subTexture_1 : register(t1);
Texture2D subTexture_2 : register(t2);
Texture2D subTexture_3 : register(t3);
Texture2D subTexture_4 : register(t4);
Texture2D subTexture_5 : register(t5);


SamplerState mainSampler : register(s0);


float3 GetModelPos() {
    return float3(-modelMatrix[0][3], -modelMatrix[1][3], -modelMatrix[2][3]);
}

//PostEffect Functions
float3 RadialBlur(float2 uv) {
    float3 col = float3(0.0, 0.0, 0.0);
    float2 d = (float2(0.5, 0.5) - uv) / 32.;
    float w = 1.0;
    float2 s = uv;
    for (int i = 0; i < 32; i++)
    {
        float3 res = (mainTexture.Sample(mainSampler, float2(s.x, s.y)).xyz);
        col += w * smoothstep(0.0, 1.0, res);
        w *= .985;
        s += d;
    }
    col = col * 4.5 / 32.;
    return 0.4 * float3(0.2 * col + 0.8 * (mainTexture.Sample(mainSampler, uv).xyz));
}


float WhiteNoise(float2 coord) {
    return frac(sin(dot(coord, float2(1.9898, 8.233))) * 437.645);
}


#define FXAA_REDUCE_MIN   (0.0078125)//1.0/128
#define FXAA_REDUCE_MUL   (0.125)//1.0/8.0
#define FXAA_SPAN_MAX     8.0

float3 FxaaPixelShader(float2 posPos, Texture2D tex, float2 rcpFrame)
{
    float2 uv = posPos + float2(0, -rcpFrame.y);
    float3 rgbN = tex.Sample(mainSampler, uv).xyz;
    uv = posPos + float2(-rcpFrame.x, 0);
    float3 rgbW = tex.Sample(mainSampler, uv).xyz;
    uv = posPos + float2(0, rcpFrame.y);
    float3 rgbS = tex.Sample(mainSampler, uv).xyz;
    uv = posPos + float2(rcpFrame.x, 0);
    float3 rgbE = tex.Sample(mainSampler, uv).xyz;
    float3 rgbM = tex.Sample(mainSampler, posPos).xyz;

    uv = posPos + float2(-rcpFrame.x, -rcpFrame.y);
    float3 rgbNW = (rgbN + rgbW + tex.Sample(mainSampler, uv).xyz + rgbM) * 0.25;
    uv = posPos + float2(rcpFrame.x, -rcpFrame.y);
    float3 rgbNE = (rgbN + rgbE + tex.Sample(mainSampler, uv).xyz + rgbM) * 0.25;
    uv = posPos + float2(-rcpFrame.x, rcpFrame.y);
    float3 rgbSW = (rgbS + rgbW + tex.Sample(mainSampler, uv).xyz + rgbM) * 0.25;
    uv = posPos + float2(rcpFrame.x, rcpFrame.y);
    float3 rgbSE = (rgbS + rgbE + tex.Sample(mainSampler, uv).xyz + rgbM) * 0.25;

    /*---------------------------------------------------------*/
    float3 luma = float3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    /*---------------------------------------------------------*/
    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
        max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
            dir * rcpDirMin)) * rcpFrame;
    /*--------------------------------------------------------*/
    float3 rgbA = 0.5 * (
        tex.Sample(mainSampler, posPos + dir * (-0.1666666666666667)).xyz +
        tex.Sample(mainSampler, posPos + dir * (0.1666666666666667)).xyz);
    float3 rgbB = rgbA * (0.5) + (0.25) * (
        tex.Sample(mainSampler, posPos + dir * (-0.5)).xyz +
        tex.Sample(mainSampler, posPos + dir * (0.5)).xyz);
    float lumaB = dot(rgbB, luma);

    bool step = ((lumaB < lumaMin) || (lumaB > lumaMax));

    return rgbA * step + rgbB * (1 - step);

}


//Vertex Definition
struct Vertex_UV
{
    float4 position : POSITION;
    float2 uv:TEXCOORD;
};
struct Vertex_UV_Color
{
    float4 position : POSITION;
    float2 uv:TEXCOORD;
    float4 color:COLOR;
};

struct Vertex_Normal
{
	//頂点シェーダのインプット構造体
	float4 position : POSITION;//ポシション
	float3 normal : NORMAL;
};


struct Vertex_UV_Normal
{
    //頂点シェーダのインプット構造体
    float4 position : POSITION;//ポシション
    float2 uv:TEXCOORD;/*UV*/
    float3 normal : NORMAL;
};


struct Vertex_UV_Normal_SingleBone
{
    //頂点シェーダのインプット構造体
    float4 position : POSITION;//ポシション
    float2 uv:TEXCOORD;/*UV*/
    float3 normal : NORMAL;
    min16int bone : BONE;
}; 

struct Vertex_UV_Normal_QuadBone
{
    float4 position : POSITION;
    float2 uv:TEXCOORD;
    float3 normal : NORMAL;
    min16int boneIndex_1 :BONEINDEXONE;
    min16int boneIndex_2 : BONEINDEXTWO;
    min16int boneIndex_3 : BONEINDEXTHREE;
    min16int boneIndex_4 : BONEINDEXFOUR;
    float weight_01 : BONEWEIGHTONE;
    float weight_02 : BONEWEIGHTTWO;
    float weight_03 : BONEWEIGHTTHREE;
    float weight_04 : BONEWEIGHTFOUR;
};

struct Vertex_PMX
{
    float4 position : POSITION;
    float2 uv:TEXCOORD;
    float3 normal : NORMAL;
    min16int boneIndex_1 :BONEINDEXONE;
    min16int boneIndex_2 : BONEINDEXTWO;
    min16int boneIndex_3 : BONEINDEXTHREE;
    min16int boneIndex_4 : BONEINDEXFOUR;
    float weight_01 : BONEWEIGHTONE;
    float weight_02 : BONEWEIGHTTWO;
    float weight_03 : BONEWEIGHTTHREE;
    float weight_04 : BONEWEIGHTFOUR;
    float3 SDEF_C:SDEFC;
    float3 SDEF_R0:SDEFRZERO;
    float3 SDEF_R1:SDEFRONE;
};

struct Vertex_UV_Normal_Color
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 color:COLOR;
};

//Pixel Definition
struct Pixel
{
    float4 position : SV_POSITION;
};
struct Pixel_UV
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
};

struct Pixel_Normal
{
	float4 position : SV_POSITION;
	float3 normal :  NORMAL;
};

struct Pixel_UV_Normal
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
    float3 normal :  NORMAL;
};
struct Pixel_OutLine
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
    float3 normal :  NORMAL;
    bool isOutLine : OUTLINE;
};

struct Pixel_UV_Normal_Color
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 color:COLOR;
};

struct Pixel_UV_Color {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color:COLOR;
};

struct Pixel_Output {
    float4 color:SV_Target;
    float depth : SV_Depth;

};



struct Pixel_Fog
{
    float4 position : SV_POSITION;
    float fog : COLOR0;
};
struct Pixel_UV_Fog
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
    float fog : COLOR0;
};

struct Pixel_Normal_Fog
{
    float4 position : SV_POSITION;
    float3 normal :  NORMAL; 
    float fog : COLOR0;
};

struct Pixel_UV_Normal_Fog
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
    float3 normal :  NORMAL;
    float fog : COLOR0;
};
struct Pixel_UV_Normal_Phong
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
    float3 normal :  NORMAL;
    float4 modelPosition : MODEL_POSITION;
};
struct Pixel_OutLine_Fog
{
    float4 position : SV_POSITION;
    float2 uv:TEXCOORD;
    float3 normal :  NORMAL;
    bool isOutLine : OUTLINE;
    float fog : COLOR0;
};

struct Pixel_UV_Normal_Color_Fog
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 color:COLOR;
    float fog : COLOR1;
};

struct Pixel_UV_Color_Fog {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color:COLOR;
    float fog : COLOR1;
};
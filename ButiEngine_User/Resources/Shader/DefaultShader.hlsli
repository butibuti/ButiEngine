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
}
cbuffer Light : register(b3)
{
	float4 lightDir	: packoffset(c0);
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


SamplerState mainSampler : register(s0);


float3 GetCameraPos() {
    matrix inversed = inverse(viewMatrix);
    return float3(inversed[0][3],inversed[1][3], inversed[2][3]);
}
float3 GetModelPos() {
    return float3(-modelMatrix[0][3], -modelMatrix[1][3], -modelMatrix[2][3]);
}

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
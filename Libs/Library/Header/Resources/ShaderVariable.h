#pragma once
#include"stdafx.h"
namespace ButiEngine {

	struct ShaderVariable
	{
		Matrix4x4 World;
		Matrix4x4 View;
		Matrix4x4 Projection;
		ShaderVariable() {
			memset(this, 0, sizeof(ShaderVariable));
		};
	};
	struct LightVariable {
		Vector4 LightDir;
		LightVariable() {
			memset(this, 0, sizeof(LightVariable));
		};
	};

	struct MaterialVariable {
		Vector4 emissive;
		Vector4 diffuse;
		Vector4 ambient;
		Vector4 specular;
		MaterialVariable() {
			memset(this, 0, sizeof(MaterialVariable));
		};
	};

	struct TestGSVariable {
		Vector4 pushPower;
		float bottom = 0;
		TestGSVariable() {
			memset(this, 0, sizeof(TestGSVariable));
		}
	};
}
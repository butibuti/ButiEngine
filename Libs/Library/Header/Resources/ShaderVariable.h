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
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(World);
			archive(View);
			archive(Projection);
		}
		bool ShowUI() { return false; }
	};
	struct LightVariable {
		Vector4 lightDir;// = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);
		LightVariable() {
			//memset(this, 0, sizeof(LightVariable));
		};
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(lightDir);
		}

		bool ShowUI() { 
			
			float dir[] = { lightDir.x,lightDir.y,lightDir.z,lightDir.w};
			if (ImGui::DragFloat4("Direction", dir, 0.02f, -500.0f, 500.0f)) {
				lightDir = Vector4(dir[0], dir[1], dir[2], dir[3]);
				return true;
			}
			return false;
		}
	};

	struct MaterialVariable {
		Vector4 emissive;
		Vector4 diffuse;
		Vector4 ambient;
		Vector4 specular;
		MaterialVariable() {
			memset(this, 0, sizeof(MaterialVariable));
		};
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(emissive);
			archive(diffuse);
			archive(ambient);
			archive(specular);
		}
		bool ShowUI() { return false; }
	};

	struct TestGSVariable {
		Vector4 pushPower;
		float bottom = 0;
		TestGSVariable() {
			memset(this, 0, sizeof(TestGSVariable));
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(bottom);
			archive(pushPower);
		}

		bool ShowUI() { 

			bool ret = false;
			if (ImGui::DragFloat("Time", &pushPower.w, 0.02f, 0.0f, 10.0f)) {

				ret = true;
			}
			if (ImGui::DragFloat("FloorHight", &bottom, 0.02f, -100.0f, 100.0f)) {

				ret = true;
			}
			return ret;
		}
	};
}

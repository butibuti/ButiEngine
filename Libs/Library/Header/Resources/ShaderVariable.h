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
			
			if (ImGui::DragFloat4("Direction",&lightDir.x, 0.01f, -500.0f, 500.0f)) {
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
		};
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(emissive);
			archive(diffuse);
			archive(ambient);
			archive(specular);
		}
		bool ShowUI() { 

			bool ret = false;
			if (ImGui::DragFloat4("Emissive", &emissive.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (ImGui::DragFloat4("Diffuse", &diffuse.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (ImGui::DragFloat4("Specular", &specular.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (ImGui::DragFloat4("Ambient", &ambient.x, 0.01f, 0.0f, 1.0f)) {
				ret= true;
			}
			return ret; 
		}
	};

	struct TestGSVariable {
		Vector4 pushPower;
		float bottom = 0;
		TestGSVariable() {
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
	struct ParticleParameter {
		Vector4 startColor=Vector4(0,0,0,1);
		Vector4 endColor = Vector4(1, 1, 1, 1);
		float time=0.0f ;
		float power=0.0f ;
		int noise=0 ;

		ParticleParameter(){}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(startColor);
			archive(endColor);
			archive(time);
			archive(power);
			archive(noise);
		}

		bool ShowUI() {

			bool ret = false;
			if (ImGui::DragFloat("Time", &time, 0.02f, 0.0f, 200.0f)) {

				ret = true;
			}
			if (ImGui::DragFloat("Power", &power, 0.02f, -100.0f, 100.0f)) {

				ret = true;
			}
			if (ImGui::DragInt("Noise", &noise, 1.0f, 0, 100)) {

				ret = true;
			}
			
			if (ImGui::ColorEdit4("StartColor", &startColor.x)) {
				ret = true;
			}

			if (ImGui::ColorEdit4("EndColor", &endColor.x)) {
				ret = true;
			}

			return ret;
		}
	};
}

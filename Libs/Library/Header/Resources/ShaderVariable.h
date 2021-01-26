#pragma once
#include"../Common/ButiMath.h"
namespace ButiEngine {


	struct ShaderVariable
	{
		Matrix4x4 World;
		Matrix4x4 View;
		Matrix4x4 Projection;
		Matrix4x4 MVP;
		ShaderVariable() {
			memset(this, 0, sizeof(ShaderVariable));
		};
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(World);
			archive(View);
			archive(Projection);
			archive(MVP);
		}
		bool ShowUI() { return false; }
	};


	struct Fog {

		Vector4 fogColor;
		Vector4 cameraPos;
		Vector2 fogCoord;
		Vector2 pixelScale;
		bool ShowUI() { return false; }

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(fogColor);
			archive(cameraPos);
			archive(fogCoord);
		}
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
			
			if (GUI::DragFloat4("Direction",&lightDir.x, 0.01f, -500.0f, 500.0f)) {
				return true;
			}
			return false;
		}
	};
	struct GausVariable {

		Vector3 gausOffset[16];
		int sampleCount = 16;
		float GaussianDistribution(const Vector2& pos, float rho)
		{
			 return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
		}
		void CalcGaus(const int width,const int height,const Vector2& dir,const float deviation) {
			auto tu = 1.0f / float(width);
			auto tv = 1.0f / float(height);
			sampleCount = 15;
			gausOffset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation);
			auto total_weight = gausOffset[0].z;
			
			gausOffset[0].x = 0.0f;
			gausOffset[0].y = 0.0f;
			
			for (auto i = 1; i < 8;i++)
			{
			        gausOffset[i].x = dir.x * i * tu;
			        gausOffset[i].y = dir.y * i * tv;
			        gausOffset[i].z = GaussianDistribution(dir * float(i), deviation);
			        total_weight += gausOffset[i].z * 2.0f;
			    }
			
			for (auto i = 0; i < 8; i++)
			{
			        gausOffset[i].z /= total_weight;
			}
			for (auto i = 8; i < 15; i++)
			{
			        gausOffset[i].x = -gausOffset[i - 8].x;
			        gausOffset[i].y = -gausOffset[i - 8].y;
			        gausOffset[i].z = gausOffset[i - 8].z;
			}
			
		}
		bool ShowUI() {
			static float deviation=0.0f;
			static Vector2 dir = Vector2(1,0);
			static Vector2 scale = Vector2(480,270);
			bool output = false;
			GUI::Text("Deviation");
			if (GUI::DragFloat("##deviation", deviation, 0.1f, 0.0f, 100.0f)) {
				output = true;
			}
			GUI::Text("Direction");
			if (GUI::DragFloat2("##dir", dir, 0.1f, 0.0f, 100.0f)) {
				output = true;
			}
			GUI::Text("Scale");
			if (GUI::DragFloat2("##scale", scale, 0.1f, 0.0f, 100.0f)) {
				output = true;
			}

			if (output) {
				CalcGaus(scale.x, scale.y, dir, deviation);
			}

			return output;
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(sampleCount);
			archive(gausOffset);
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
			if (GUI::DragFloat4("Emissive", &emissive.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (GUI::DragFloat4("Diffuse", &diffuse.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (GUI::DragFloat4("Specular", &specular.x, 0.01f, 0.0f, 1.0f)) {
				ret = true;
			}
			if (GUI::DragFloat4("Ambient", &ambient.x, 0.01f, 0.0f, 1.0f)) {
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
			if (GUI::DragFloat("Time", &pushPower.w, 0.02f, 0.0f, 10.0f)) {

				ret = true;
			}
			if (GUI::DragFloat("FloorHight", &bottom, 0.02f, -100.0f, 100.0f)) {

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
		float outer=20.0f;
		float center=1.0f;
		float size=1.0f;
		float minSize=0.1f;
		float rotationPase = 5.0f;

		ParticleParameter(){}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(startColor);
			archive(endColor);
			archive(time);
			archive(power);
			archive(noise);
			archive(outer);
			archive(center);
			archive(size);
			archive(minSize);
			archive(rotationPase);
		}

		bool ShowUI();
	};
}

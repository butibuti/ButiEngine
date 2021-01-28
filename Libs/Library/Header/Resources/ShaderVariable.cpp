#include "stdafx.h"
#include "ShaderVariable.h"

bool ButiEngine::ParticleParameter::ShowUI()
{
	{

		bool ret = false;
		if (GUI::DragFloat("Time", &time, 0.02f, 0.0f, 200.0f)) {

			ret = true;
		}
		if (GUI::DragFloat("Power", &power, 0.02f, -100.0f, 100.0f)) {

			ret = true;
		}
		if (GUI::DragInt("Noise", noise, 1.0f, 0, 10)) {

			ret = true;
		}
		if (GUI::DragFloat("Center", center, 0.02f, -100.0f, 100.0f)) {

			ret = true;
		}
		if (GUI::DragFloat("Outer", outer, 0.02f, -100.0f, 100.0f)) {

			ret = true;
		}
		if (GUI::DragFloat("Size", &size, 0.02f, -100.0f, 100.0f)) {

			ret = true;
		}
		if (GUI::DragFloat("MinSize", &minSize, 0.02f, -100.0f, 100.0f)) {

			ret = true;
		}
		if (GUI::DragFloat("RotationPase", &rotationPase, 0.02f, -100.0f, 100.0f)) {

			ret = true;
		}
		if (GUI::ColorEdit4("StartColor", startColor)) {

			ret = true;
		}
		if (GUI::ColorEdit4("EndColor", endColor)) {

			ret = true;
		}


		return ret;
	}
}

float ButiEngine::GausVariable::GaussianDistribution(const Vector2& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}
void ButiEngine::GausVariable::CalcGaus(const int width, const int height, const Vector2& dir, const float deviation)
{
	auto tu = 1.0f / float(width);
	auto tv = 1.0f / float(height);
	gausOffset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation);
	auto total_weight = gausOffset[0].z;
	
	gausOffset[0].x = 0.0f;
	gausOffset[0].y = 0.0f;
	
	for (auto i = 1; i < 8; ++i)
		{
		  gausOffset[i].x = dir.x * i *tu;
	      gausOffset[i].y = dir.y * i *tv;
	      gausOffset[i].z = GaussianDistribution(dir * float(i), deviation);
	      total_weight += gausOffset[i].z * 2.0f;
	  }
	
	for (auto i = 0; i < 8; ++i)
	{
	     gausOffset[i].z /= total_weight;
	}
	//gausOffset[0].z = 1.0f;
	for (auto i = 8; i < 15; ++i)
	{
	    gausOffset[i].x = -gausOffset[i - 7].x;
	    gausOffset[i].y = -gausOffset[i - 7].y;
		gausOffset[i].z = gausOffset[i - 7].z;
	}

}

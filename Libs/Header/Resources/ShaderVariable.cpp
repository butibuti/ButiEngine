#include "stdafx.h"

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

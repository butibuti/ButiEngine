#pragma once
#include"stdafx.h"
#include"Header/GameObjects/DefaultBehavior/SampleBehavior.h"
void ButiEngine::SampleBehavior::OnUpdate()
{
	if (GameDevice::input.CheckKey(Keys::J)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(1.0f);
	}
	if (GameDevice::input.CheckKey(Keys::L)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-1.0f);
	}
	if (GameDevice::input.CheckKey(Keys::I)) {
		gameObject.lock()->transform->RollLocalRotationX_Degrees(1.0f);
	}
	if (GameDevice::input.CheckKey(Keys::K)) {
		gameObject.lock()->transform->RollLocalRotationX_Degrees(-1.0f);
	}
	if (GameDevice::input.CheckKey(Keys::N)) {
		gameObject.lock()->transform->RollLocalRotationZ_Degrees(1.0f);
	}
	if (GameDevice::input.CheckKey(Keys::M)) {
		gameObject.lock()->transform->RollLocalRotationZ_Degrees(-1.0f);
	}

}

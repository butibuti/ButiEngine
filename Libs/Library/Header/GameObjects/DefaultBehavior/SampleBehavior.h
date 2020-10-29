#pragma once
#include"Header/GameObjects/Behavior.h"
namespace ButiEngine {

	class SampleBehavior:public Behavior
	{
		std::string GetBehaviorName()override {
			return "SampleBehavior";
		}
		void OnUpdate()override;
	};

}
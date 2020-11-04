#pragma once
#include"Header/GameObjects/Behavior.h"
namespace ButiEngine {

	class SampleBehavior:public Behavior
	{
		std::string GetBehaviorName()override {
			return "SampleBehavior";
		}
		void OnUpdate()override;
		void OnSet()override;
		std::shared_ptr<Behavior> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	};

}
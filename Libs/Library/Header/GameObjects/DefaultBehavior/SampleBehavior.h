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
		void Start()override;
		std::shared_ptr<Behavior> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI()override;
	private:
		float t = 0;
		std::shared_ptr<CBuffer<TestGSVariable> >cb_barabara;
	};

}


//BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);
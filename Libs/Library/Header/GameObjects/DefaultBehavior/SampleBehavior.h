#pragma once
#include"Header/GameObjects/Behavior.h"
namespace ButiEngine {



	class SampleBehavior:public Behavior
	{
	public:
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
		void PositionSet();
	private:
		
		int t = 0;
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);
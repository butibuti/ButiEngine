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
		
		SplineCurve splineCurve;
		float t = 0;
		float speed = 0.0025f;;
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);
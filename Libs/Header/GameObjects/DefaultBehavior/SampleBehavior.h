#pragma once
#include"../../BehaviorHeader.h"
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
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnd(std::weak_ptr<GameObject> arg_other)override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI()override;
		void PositionSet();
	private:
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);
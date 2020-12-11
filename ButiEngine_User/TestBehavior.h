#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {


	class TestBehavior :public Behavior
	{
	public:
		std::string GetBehaviorName()override {
			return "TestBehavior";
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
	private:
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::TestBehavior);
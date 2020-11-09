#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	class FPSViewBehavior :public Behavior
	{
	public:
		void Start()override;
		void OnUpdate() override;
		void OnSet()override;
		std::string GetBehaviorName()override {
			return "FPSViewBehavior";
		};
		std::shared_ptr<Behavior> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::weak_ptr<GameObject> head;
		bool isCenter = true;
	};

}
BUTI_REGIST_BEHAVIOR(ButiEngine::FPSViewBehavior);

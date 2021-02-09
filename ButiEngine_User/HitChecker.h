#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	class HitChecker :public Behavior
	{
	public:
		HitChecker() {}

		void OnUpdate()override;
		std::string GetBehaviorName()override {
			return "HitChecker";
		}
		std::shared_ptr<Behavior> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void ShowGUI()override;
	private:

		std::string hitObjName;
	};

}
BUTI_REGIST_BEHAVIOR(ButiEngine::HitChecker)
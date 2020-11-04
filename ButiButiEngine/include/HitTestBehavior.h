#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	class HitTestBehavior:public Behavior
	{
	public:
		std::string GetBehaviorName()override {
			return "HitTestBehavior";
		}
		void OnUpdate()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnd(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<Behavior> Clone()override;
	};

}
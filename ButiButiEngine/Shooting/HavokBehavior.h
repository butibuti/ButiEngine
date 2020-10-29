#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	class HavokBehavior:public Behavior
	{
	public:
		void OnUpdate()override;
		void OnSet()override;
		void Trigger();
		void BulletShot();
		std::string GetBehaviorName()override {
			return"HavokBehavior";
		}
	private:
		std::shared_ptr<TimeBomb<HavokBehavior>> shp_coolTimer;

	};

}
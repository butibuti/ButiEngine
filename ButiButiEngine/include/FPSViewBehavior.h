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
			return "PlayerBehavior";
		};
	private:
		std::weak_ptr<GameObject> head;
		std::shared_ptr<CBuffer<TestGSVariable>> shp_testGS;
		int boneIndex = 0;
		bool isCenter=true;
	};

}
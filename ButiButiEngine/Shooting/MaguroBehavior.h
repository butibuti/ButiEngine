#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	class HavokBehavior;
	class MaguroBehavior:public Behavior
	{
	public:
		void Start()override;
		void OnUpdate() override;
		void OnSet()override;
		std::string GetBehaviorName()override {
			return "MaguroBehavior";
		};
	private:
		std::shared_ptr<Transform> targetTrans;
		std::shared_ptr<HavokBehavior> shp_havokBehavior;
		std::shared_ptr<Transform> lookTargetTrans;
		std::weak_ptr<GameObject> havok;
		std::vector<std::shared_ptr<Transform>> vec_maguroTaleBonesTransform; 
		std::vector<std::shared_ptr<Transform>> vec_maguroHeadBonesTransform;
		float speed = 0.08f;
		float taleSwing = 0.0f;
		float headSwing=0.0f;
		char swingVeloc = 1;
	};

}
#pragma once
#include"Header/BehaviorHeader.h"
#include"Header/Common/CollisionPrimitive.h"
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
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<Behavior> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::TestBehavior);
#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionPrimitive_Box_AABB;
	}

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
		std::shared_ptr<ICamera> shp_camera;
		Vector3 accel = Vector3();
		std::shared_ptr<RelativeTimer>shp_timer;
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::TestBehavior);
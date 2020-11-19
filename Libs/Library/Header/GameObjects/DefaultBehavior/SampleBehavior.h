#pragma once
#include"Header/GameObjects/Behavior.h"
namespace ButiEngine {


	class Resource_RealTimeMesh;
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
		std::shared_ptr<Resource_RealTimeMesh> shp_mesh;
		int t = 0;
	};

}

BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);
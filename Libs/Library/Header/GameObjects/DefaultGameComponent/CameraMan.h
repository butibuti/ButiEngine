#pragma once
#include "../../BehaviorHeader.h"

namespace ButiEngine {
	class CameraMan :public Behavior {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetBehaviorName() {
			return "CameraMan";
		}
		void ShakeVartical(const float power);
		void ShakeHorizontal(const float power);
		template<class Archive>
		void serialize(Archive& archive)
		{
			//archive(isActive);
			//archive(cameraName);
		}
		std::shared_ptr<Behavior> Clone()override {
			return ObjectFactory::Create<CameraMan>();
		}
		void OnShowUI()override;
	private:
		float vertQuake;
		static float vertQuakeMax;
		float horizonQuake;
		static float horizonQuakeMax;
		std::shared_ptr<Transform> child;
		std::string cameraName="main";
	};
}

BUTI_REGIST_BEHAVIOR(ButiEngine::CameraMan)
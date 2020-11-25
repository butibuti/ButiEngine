#include "stdafx.h"
#include "Header/BehaviorHeader.h"

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
		}
		std::shared_ptr<Behavior> Clone()override {
			return ObjectFactory::Create<CameraMan>();
		}
	private:
		float vertQuake;
		static float vertQuakeMax;
		float horizonQuake;
		static float horizonQuakeMax;
		std::shared_ptr<Transform> child;
	};
}

BUTI_REGIST_BEHAVIOR(ButiEngine::CameraMan)
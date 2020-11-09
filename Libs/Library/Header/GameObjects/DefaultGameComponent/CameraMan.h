#include "stdafx.h"
#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class CameraMan :public Behavior {
	public:
		void Start() override;
		virtual std::string GetBehaviorName() {
			return "CameraMan";
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
		}
		std::shared_ptr<Behavior> Clone()override {
			return ObjectFactory::Create<CameraMan>();
		}
	private:
	};
}

BUTI_REGIST_BEHAVIOR(ButiEngine::CameraMan)
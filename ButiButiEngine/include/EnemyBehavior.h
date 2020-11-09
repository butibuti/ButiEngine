#include "stdafx.h"
#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class EnemyBehavior :public Behavior {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetBehaviorName() {
			return "EnemyBehavior";
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(velocity);
			archive(speed);
		}

		std::shared_ptr<Behavior> Clone()override {
			return ObjectFactory::Create<EnemyBehavior>();
		}
	private:
		Vector3 velocity;
		float speed;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::EnemyBehavior);
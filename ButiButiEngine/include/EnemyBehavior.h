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

		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(velocity);
			archive(speed);
			archive(hp);
		}

		std::shared_ptr<Behavior> Clone()override {
			return ObjectFactory::Create<EnemyBehavior>();
		}
		void OnShowUI()override;
	private:
		Vector3 velocity = Vector3();
		Vector3 moveForce=Vector3();
		float speed;
		UINT hp=0;

		std::shared_ptr<CBuffer<TestGSVariable> >cb_barabara;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::EnemyBehavior);
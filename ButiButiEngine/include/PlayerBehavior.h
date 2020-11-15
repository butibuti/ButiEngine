
#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class PlayerBehavior :public Behavior {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetBehaviorName() {
			return "PlayerBehavior";
		}

		std::shared_ptr<Behavior> Clone()override {
			return ObjectFactory::Create<PlayerBehavior>();
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(speed);
			archive(controllPase);
			archive(inertiaMinorPase);
		}

		void OnShowUI()override;

		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;

	private:
		float speed;
		float controllPase;
		float inertiaMinorPase;
		int hp = 5;
		Vector3 inertia;
		Vector3 initPos;
		Vector3 moveForce;
		Matrix4x4 initRotate;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::PlayerBehavior);
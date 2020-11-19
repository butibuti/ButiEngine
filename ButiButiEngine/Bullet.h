#include "stdafx.h"
#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class Bullet :public Behavior {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetBehaviorName() {
			return "Bullet";
		}

		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(velocity);
			archive(speed);
		}

		std::shared_ptr<Behavior> Clone()override {
			auto enemyBeh = ObjectFactory::Create<Bullet>();
			enemyBeh->speed = speed;
			enemyBeh->velocity = velocity;
			return enemyBeh;
		}
		void OnShowUI()override;

		void SetVelocity(const Vector3& arg_velocity);
		void SetSpeed(const float arg_speed);

	private:
		Vector3 velocity = Vector3();
		float speed;

		std::shared_ptr<CBuffer<TestGSVariable> >cb_barabara;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::Bullet);
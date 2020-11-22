#include "stdafx.h"
#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class GameController;
	class BossEnemy :public Behavior {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetBehaviorName() {
			return "BossEnemy";
		}

		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(velocity);
			archive(speed);
			archive(hp);
			archive(score);
		}

		std::shared_ptr<Behavior> Clone()override {
			auto enemyBeh= ObjectFactory::Create<BossEnemy>();
			enemyBeh->hp = hp;
			enemyBeh->speed = speed;
			enemyBeh->velocity = velocity;
			enemyBeh->score = score;
			return enemyBeh;
		}
		void OnShowUI()override;
	private:
		Vector3 velocity = Vector3();
		Vector3 moveForce=Vector3();
		Vector3 stagemin;
		Vector3 stagemax;
		float speed;
		UINT hp=0;
		int score = 0;
		std::shared_ptr< GameController> controller;

		std::shared_ptr<CBuffer<TestGSVariable> >cb_barabara;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::BossEnemy);

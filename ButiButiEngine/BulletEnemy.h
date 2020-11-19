#include "stdafx.h"
#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class GameController;
	class BulletEnemy :public Behavior {
	public:
		void Start() override;
		void OnSet()override;
		void OnUpdate()override;
		virtual std::string GetBehaviorName() {
			return "BulletEnemy";
		}

		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(velocity);
			archive(speed);
			archive(hp);
			archive(score);
			archive(shp_timer);
		}

		std::shared_ptr<Behavior> Clone()override {
			auto enemyBeh = ObjectFactory::Create<BulletEnemy>();
			enemyBeh->hp = hp;
			enemyBeh->speed = speed;
			enemyBeh->velocity = velocity;
			enemyBeh->shp_timer = shp_timer->Clone();
			return enemyBeh;
		}
		void OnShowUI()override;
	private:
		Vector3 velocity = Vector3();
		Vector3 moveForce = Vector3();
		Vector3 stagemin;
		Vector3 stagemax;
		float speed;
		std::shared_ptr<RelativeTimer> shp_timer;
		UINT hp = 0;
		float size = 1.0f;
		int score=0;

		std::shared_ptr< GameController> controller;
		std::shared_ptr<CBuffer<TestGSVariable> >cb_barabara;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::BulletEnemy);

#include "Header/BehaviorHeader.h"

namespace ButiEngine {
	class GameController;
	class ImmediateParticleController;
	class CameraMan;
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

		void SetIsInvisible(const bool arg_isInv) {
			isInvisible = arg_isInv;
		}
	private:
		float speed;
		float controllPase;
		float inertiaMinorPase;
		Vector3 stagemin;
		Vector3 stagemax;
		int hp =3;
		Vector3 inertia;
		Vector3 initPos;
		Vector3 moveForce;
		Matrix4x4 initRotate;
		std::shared_ptr< GameController> controller;
		std::shared_ptr<RelativeTimer> damageInvTimer;
		std::shared_ptr<CameraMan> shp_cameraman;
		bool isInvisible = false;
	};
}
BUTI_REGIST_BEHAVIOR(ButiEngine::PlayerBehavior);
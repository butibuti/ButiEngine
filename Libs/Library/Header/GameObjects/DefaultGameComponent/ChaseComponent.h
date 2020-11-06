#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class ChaseComponent:public GameComponent
	{
	public:
		ChaseComponent(std::shared_ptr<Transform> arg_shp_target,const float arg_speed=0.1f);
		ChaseComponent(){}
		void OnUpdate() override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "ChaseBehavior";
		};
		void OnShowUI()override;

		std::shared_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_target);
			archive(speed);
			archive(isActive);
		}

	private:
		std::shared_ptr<Transform> shp_target;
		float speed;
	};

}


CEREAL_REGISTER_TYPE(ButiEngine::ChaseComponent);

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, ButiEngine::ChaseComponent)
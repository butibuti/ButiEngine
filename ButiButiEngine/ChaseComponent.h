#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class ChaseComponent:public GameComponent
	{
	public:
		ChaseComponent(std::shared_ptr<Transform> arg_shp_target,const float arg_speed=0.001f);
		void OnUpdate() override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "ChaseBehavior";
		};
	private:
		std::shared_ptr<Transform> shp_target;
		float speed;
	};

}
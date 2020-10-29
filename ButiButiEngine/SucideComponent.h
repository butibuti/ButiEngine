#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class SucideComponent:public GameComponent
	{
	public:
		SucideComponent(const float arg_float);
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "SicideComponent";
		}
	private:
		std::shared_ptr<RelativeTimer> shp_timer;
	};

}
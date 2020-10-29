#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class LookAtComponent :public GameComponent
	{
	public:
		LookAtComponent(std::shared_ptr<Transform> arg_shp_lookTarget);

		void OnUpdate()override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "LookAtComponent";
		}
	private:

		std::shared_ptr<Transform> shp_lookTarget;
	};

}
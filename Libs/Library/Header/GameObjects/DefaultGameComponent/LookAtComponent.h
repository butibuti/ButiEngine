#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class LookAtComponent :public GameComponent
	{
	public:
		LookAtComponent(std::shared_ptr<Transform> arg_shp_lookTarget);
		LookAtComponent(){}

		void OnUpdate()override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "LookAtComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_lookTarget);
			archive(isActive);
		}
	private:
		std::shared_ptr<Transform> shp_lookTarget;
	};

}



CEREAL_REGISTER_TYPE(ButiEngine::LookAtComponent);

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, ButiEngine::LookAtComponent)
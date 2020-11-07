#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class SucideComponent:public GameComponent
	{
	public:
		SucideComponent(const float arg_count);
		SucideComponent() {}
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "SucideComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(count);
			archive(isActive);
		}
	private:
		std::shared_ptr<RelativeTimer> shp_timer;
		float count;
	};

}



CEREAL_REGISTER_TYPE(ButiEngine::SucideComponent);

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, ButiEngine::SucideComponent)
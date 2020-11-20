#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class SucideComponent:public GameComponent
	{
	public:
		SucideComponent(const float arg_count);
		SucideComponent();
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "SucideComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(count);
			archive(shp_timer);
			archive(isActive);
		}
	private:
		std::shared_ptr<RelativeTimer> shp_timer;
		float count;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::SucideComponent);
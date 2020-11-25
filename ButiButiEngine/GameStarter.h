#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class GameStarter :public GameComponent
	{
	public:
		void Start()override;
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "GameStarter";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(requestPush);
			archive(shp_relTimer);
		}
		void OnShowUI();
		void OnSet();
	private:
		int pushCount=0;
		int requestPush = 1;
		std::shared_ptr<RelativeTimer> shp_relTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::GameStarter);

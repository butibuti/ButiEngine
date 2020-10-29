#pragma once
#include "Header/Scene/Scene.h"
namespace ButiEngine {
	class ShootingScene:public Scene
	{
	public:
		ShootingScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation sceneInformation);
		~ShootingScene();
		void OnUpdate() override;
		void OnInitialize()override;
		void PreInitialize()override;
		void OnSceneEnd()override;
		bool isTopView = true;
	};

}
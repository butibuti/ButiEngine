#pragma once
#ifndef _SceneManager_H_
#define _SceneManager_H_

#include"stdafx.h"
namespace ButiEngine {
	class IApplication;
	class SceneManager :public ISceneManager
	{
	public:
		SceneManager(std::weak_ptr<IApplication> arg_wkp_app);

		void Update()override;
		void Initialize()override;
		void PreInitialize()override;
		void RemoveScene(const std::string& arg_sceneName)override;
		void ChangeScene(const std::string& arg_sceneName,float sceneChangeDalay=0)override;
		void SetScene(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene)override;
		void SetScene_Init(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene)override;


		void LoadScene(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene=nullptr) override;
		void LoadScene_Init(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene=nullptr) override;
		void ReloadScene()override;
		void ReloadScene(const std::string& arg_sceneName)override;
		void RenewalScene()override;
		std::weak_ptr<IApplication> GetApplication()override;
		inline std::shared_ptr<IScene> GetCurrentScene()override {
			return currentScene;
		}
		void Release()override;
		~SceneManager();
	protected:
		std::shared_ptr<IScene> currentScene;
		std::shared_ptr<IScene> newScene;
		std::map<std::string, std::shared_ptr<IScene>> map_iscene;
		std::shared_ptr<AbsoluteTimer> sceneChangeTimer;
		std::weak_ptr<IApplication> wkp_app;
		bool isReload;
	};
}
#endif // !_SceneManager_H_



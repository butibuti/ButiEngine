#pragma once
#ifndef _SceneManager_H_
#define _SceneManager_H_

#include"stdafx.h"
namespace ButiEngine {
	class Application;
	class SceneManager :public ISceneManager
	{
	public:
		SceneManager(std::weak_ptr<Application> arg_wkp_app);

		inline void Update()override {
			if (sceneChangeTimer->Update())
			{
				RenewalScene();
				sceneChangeTimer->Stop();
			}
			return currentScene->Update();
		}
		void Initialize()override;
		void PreInitialize()override;
		void LoadScene(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene)override;
		void RemoveScene(const std::string& arg_sceneName)override;
		void ChangeScene(std::string arg_sceneName,float sceneChangeDalay=0)override;
		void SetScene_Init(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene)override;
		void RenewalScene()override;
		std::weak_ptr<Application> GetApplication()override;
		inline std::shared_ptr<IScene> GetCurrentScene()override {
			return currentScene;
		}
		void Release()override;
		~SceneManager();
		inline void Draw()override {
			currentScene->Draw();
		}
	private:
		std::shared_ptr<IScene> currentScene;
		std::shared_ptr<IScene> newScene;
		std::map<std::string, std::shared_ptr<IScene>> map_iscene;
		std::shared_ptr<AbsoluteTimer> sceneChangeTimer;
		std::weak_ptr<Application> wkp_app;
	};
}
#endif // !_SceneManager_H_



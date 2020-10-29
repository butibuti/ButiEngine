#pragma once
#include"stdafx.h"
#include "ApplicationBeffor.h"
namespace ButiEngine {
	inline std::shared_ptr<ISceneManager> ButiEngine::Application::GetSceneManager()
	{
		return shp_sceneManager;
	}
	inline std::shared_ptr<GraphicDevice> ButiEngine::Application::GetGraphicDevice()
	{
		return shp_graphicDevice;
	}
	inline std::shared_ptr<ResourceContainer> ButiEngine::Application::GetResourceContainer()
	{
		return shp_resourceContainer;
	}
	inline bool Application::Update() {
		shp_sceneManager->Update();
		shp_sceneManager->Draw();
		return unq_window->Update();
	}
	inline int Application::Run()
	{
		while (Update())
		{
			GameDevice::input.PadUpdate();
			GameDevice::input.MouseUpdate();

			if (GameDevice::input.CheckKey(Keys::Esc)) {
				return 1;
			}
#ifdef DEBUG
			if (GameDevice::input.TriggerKey(Keys::C)) {
				std::cout << "コマンドを入力してください" << std::endl;
				std::string cmd;
				std::cin>>cmd;
			}
#endif
		}
		return 0;
	}
}
#pragma once
#include"stdafx.h"
#include "ApplicationBefore.h"
namespace ButiEngine {

	const float frame_min = (1.0f / 60.0f) * 1000;

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
		unq_imguiController->Start();
		shp_sceneManager->Update();
		shp_sceneManager->Draw();
		return unq_window->Update();
	}
	inline std::unique_ptr<ButiimguiController>& ButiEngine::Application::GetGUIController()
	{
		return unq_imguiController;
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

			std::timespec_get(&nowTs, TIME_UTC);
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds{ nowTs.tv_nsec }) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds{ befTs.tv_nsec });
			if (ms.count()>0){
				auto sleepTime =frame_min  - ms.count();
				if (sleepTime > 0) {
					Sleep(sleepTime);
				}
				else {
				}
		}
			std::timespec_get(&befTs, TIME_UTC);
		}
		return 0;
	}
}
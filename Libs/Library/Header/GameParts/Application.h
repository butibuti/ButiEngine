#pragma once
#include"stdafx.h"
namespace ButiEngine {

	class ISceneManager;
	class Window;
	class ResourceContainer;



	class Application :public IApplication
	{
	public:
		Application() {};
		void Initialize()override {}
		void PreInitialize()override {}
		void CreateInstances(const std::string windowName = "Buti", const WindowPopType arg_windowPopType = WindowPopType::normal, const UINT windowWidth = 720, const UINT windowHeight = 480, const bool isFullScreen = false,const bool isEditor=false);
		std::unique_ptr<IWindow>& GetWindow()override;
		std::shared_ptr<ISceneManager> GetSceneManager()override;
		std::shared_ptr<GraphicDevice> GetGraphicDevice()override;
		std::shared_ptr<IResourceContainer> GetResourceContainer()override;
		std::unique_ptr<ImguiController>& GetGUIController()override;
		std::shared_ptr<GameObjectTagManager> GetGameObjectTagManager()override;
		bool Update()override;
		int Run()override;
		void InitLoadResources()override;
		void Exit()override;
	private:
		std::shared_ptr<GraphicDevice>shp_graphicDevice;
		std::unique_ptr<IWindow> unq_window;
		std::unique_ptr<ImguiController> unq_imguiController;
		std::shared_ptr< ISceneManager> shp_sceneManager;
		std::shared_ptr<IResourceContainer>shp_resourceContainer;
		std::shared_ptr<GameObjectTagManager> shp_gameObjTagManager;
		std::timespec befTs;
		std::timespec nowTs;
	};

}
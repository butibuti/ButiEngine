#pragma once
#include"stdafx.h"
namespace ButiEngine {

	class ISceneManager;
	class Window;
	class ResourceContainer;
	//FlyWeight renderer,window,sceneManager
	class GlobalSettings {
	public:

		static std::string GetResourceDirectory() {
			return resourceDirectory;
		}

	private:
		static std::string resourceDirectory;
	};
	class Application:public IObject
	{
	public:
		Application() {};
		void Initialize()override{}
		void PreInitialize()override{}
		void CreateInstances(const std::string windowName = "Buti",const WindowPopType arg_windowPopType=WindowPopType::normal, const UINT windowWidth=720,const UINT windowHeight=480,const bool isFullScreen=false);
		 std::unique_ptr<Window>& GetWindow();
		inline std::shared_ptr<ISceneManager> GetSceneManager();
		inline std::shared_ptr<GraphicDevice> GetGraphicDevice();
		inline std::shared_ptr<ResourceContainer> GetResourceContainer();
		inline bool Update();
		int Run();
		void InitLoadResources();
		void Exit();
	private:
		std::shared_ptr<GraphicDevice>shp_graphicDevice;
		std::unique_ptr<Window> unq_window;
		std::shared_ptr< ISceneManager> shp_sceneManager;
		std::shared_ptr<ResourceContainer>shp_resourceContainer;
	};

}
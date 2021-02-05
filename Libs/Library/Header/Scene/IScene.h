#pragma once

namespace ButiEngine {
	class ISceneManager;
	class IResourceContainer;
	class Renderer;
	class Window;

	class IScene:public IObject
	{
	public:
		virtual void Set()=0;
		virtual void Update() = 0;
		virtual void UIUpdate()=0;
		virtual void Draw() = 0;
		virtual void EditCameraUpdate() = 0;
		virtual void ActiveCollision(const UINT arg_layerCount) = 0;
		virtual std::weak_ptr<ICamera> GetCamera(const std::string& arg_camName) = 0;
		virtual std::weak_ptr<ICamera> GetCamera(const UINT arg_camNum=0) = 0;
		virtual std::weak_ptr<ICamera> AddCamera(CameraProjProperty& arg_prop, const std::string& arg_cameraName, const bool arg_initActive) = 0;
		virtual std::weak_ptr<ICollisionManager> GetCollisionManager() = 0;
		virtual void RemoveCamera(const std::string& arg_camName) = 0;
		virtual void RemoveCamera(const UINT arg_camNum) = 0;
		virtual std::shared_ptr<IRenderer> GetRenderer()=0;
		virtual std::shared_ptr<ISoundManager> GetSoundManager() = 0;
		virtual std::shared_ptr<IResourceContainer> GetResourceContainer()=0;
		virtual std::weak_ptr< ISceneManager> GetSceneManager() = 0;
		virtual std::unique_ptr<IWindow>& GetWindow() = 0;
		virtual void SceneEnd()=0;
		virtual void Release() = 0;
		virtual std::shared_ptr< SceneInformation> GetSceneInformation() = 0;
		virtual std::shared_ptr< SceneChangeInformation> GetSceneChangeInformation() = 0;
		virtual std::shared_ptr< SceneRenderingInformation> GetSceneRenderingInformation() = 0;
		virtual void Save()=0;
		virtual void CameraActivation(const bool arg_status) = 0;
		virtual void Start() = 0;
		virtual void ShowGameObjectManagerUI() = 0;
		virtual void ShowRenderingUI() = 0;
		virtual void ShowInspectorUI()=0;
		virtual void ShowHeirarcyUI() = 0;
	protected:
	};

}
#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class ISceneManager;
	class Renderer;
	class Window;
	class IScene:public IObject
	{
	public:
		virtual void Set()=0;
		virtual void Update()=0;
		virtual void Draw() = 0; 
		virtual void ActiveCollision(const UINT arg_layerCount) = 0;
		virtual std::weak_ptr<ICamera> GetCamera(const std::string& arg_camName) = 0;
		virtual std::weak_ptr<ICamera> GetCamera(const UINT arg_camNum=0) = 0;
		virtual std::weak_ptr<ICamera> AddCamera(const CameraProjProperty& arg_prop, const std::string arg_cameraName,const bool arg_initActive) = 0;
		virtual std::weak_ptr<Collision::CollisionManager> GetCollisionManager() = 0;
		virtual void RemoveCamera(const std::string& arg_camName) = 0;
		virtual void RemoveCamera(const UINT arg_camNum) = 0;
		virtual std::shared_ptr<IRenderer> GetRenderer()=0;
		virtual std::shared_ptr<ISoundManager> GetSoundManager()=0;
		virtual std::weak_ptr< ISceneManager> GetSceneManager() = 0;
		virtual std::unique_ptr<Window>& GetWindow() = 0;
		virtual void SceneEnd()=0;
		virtual void Release() = 0;
		virtual SceneInformation GetSceneInformation() = 0;
	protected:
	};

}
#pragma once
#ifndef _Scene_H_
#define _Scene_H_
#include"stdafx.h"
#include"../GameParts/CollisionManager.h"
namespace ButiEngine {

	class Scene :public IScene
	{
	public:
		Scene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, std::shared_ptr< SceneInformation> argSceneInformation);
		void Release()override;
		virtual void OnSet();
		virtual void OnInitialize() {}
		virtual void  OnUpdate() {}
		virtual void  OnSceneEnd() {}
		void ActiveCollision(const UINT arg_layerCount=1);
		void PreInitialize()override;
		std::unique_ptr<IWindow>& GetWindow()override;

		void Update()override final;
		void Set()override final;
		void Draw()override final;
		void Initialize()override final;
		std::weak_ptr<ICamera> GetCamera(const std::string& arg_camName) override final;
		std::weak_ptr<ICamera> GetCamera(const UINT arg_camNum = 0) override final;
		std::weak_ptr<ICamera> AddCamera(CameraProjProperty& arg_prop, const std::string& arg_cameraName, const bool arg_initActive) override final;
		void RemoveCamera(const std::string& arg_camName) override final;
		void RemoveCamera(const UINT arg_camNum) override final;
		void SceneEnd()override final;
		std::shared_ptr<IResourceContainer> GetResourceContainer();
		std::shared_ptr<IRenderer> GetRenderer()override;
		std::shared_ptr<ISoundManager> GetSoundManager()override;
		std::weak_ptr< ISceneManager> GetSceneManager()override;
		std::weak_ptr<Collision::CollisionManager> GetCollisionManager()override;
		std::shared_ptr< SceneInformation> GetSceneInformation()override;
		std::shared_ptr< SceneChangeInformation> GetSceneChangeInformation() override;
		std::shared_ptr< SceneRenderingInformation> GetSceneRenderingInformation()override;
	protected:
		std::shared_ptr<ISceneManager> shp_sceneManager;
		std::shared_ptr<GameObjectManager> shp_gameObjectManager;
		std::shared_ptr<Collision::CollisionManager> shp_collisionManager;
	private:
		std::shared_ptr<ISoundManager>shp_soundManager;
		std::shared_ptr<IRenderer>shp_renderer;
		std::vector< std::shared_ptr<ICamera>> vec_cameras;
		std::shared_ptr< SceneInformation> sceneInformation;
		std::shared_ptr< SceneRenderingInformation> shp_renderingInfo;
		std::shared_ptr< SceneChangeInformation> shp_changeInfo;
	};
}
#endif // !_Scene_H_


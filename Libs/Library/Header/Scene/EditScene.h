#include "stdafx.h"

#include "Header/Common/CBuffer_Dx12.h"
#include"Header/GameParts/CollisionManager.h"
namespace ButiEngine {
	class EditScene :public IScene {

	public:
		EditScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation argSceneInformation);
		void Release()override;
		virtual void OnSet();
		virtual void OnInitialize();
		virtual void  OnUpdate();

		virtual void UIUpdate();

		void EditCamera();

		virtual void  OnSceneEnd() {}
		void ActiveCollision(const UINT arg_layerCount = 1);
		void PreInitialize()override;
		std::unique_ptr<Window>& GetWindow()override;

		void Update()override final;


		void Set()override final;
		void Draw()override final;
		void Initialize()override final;
		std::weak_ptr<ICamera> GetCamera(const std::string& arg_camName) override final;
		std::weak_ptr<ICamera> GetCamera(const UINT arg_camNum = 0) override final;
		std::weak_ptr<ICamera> AddCamera(const CameraProjProperty& arg_prop, const std::string arg_cameraName, const bool arg_initActive) override final;
		void RemoveCamera(const std::string& arg_camName) override final;
		void RemoveCamera(const UINT arg_camNum) override final;
		void SceneEnd()override final;
		std::shared_ptr<ResourceContainer> GetResourceContainer();
		std::shared_ptr<IRenderer> GetRenderer()override;
		std::shared_ptr<ISoundManager> GetSoundManager()override;
		std::weak_ptr< ISceneManager> GetSceneManager()override;
		std::weak_ptr<Collision::CollisionManager> GetCollisionManager()override;
	protected:
		std::shared_ptr<ISceneManager> shp_sceneManager;
		std::shared_ptr<GameObjectManager> shp_gameObjectManager;
		std::shared_ptr<Collision::CollisionManager> shp_collisionManager;
	private:
		std::shared_ptr<ISoundManager>shp_soundManager;
		std::shared_ptr<IRenderer>shp_renderer;
		std::vector< std::shared_ptr<ICamera>> vec_cameras;
		std::shared_ptr<ICamera> mainCam;
		std::shared_ptr<ICamera> editCam;

		std::vector<std::shared_ptr<Behavior>>vec_shp_addBehavior;



		std::vector<std::shared_ptr<GameComponent>>vec_shp_addComponents;

		//////////////////////////////////////
		bool isActive = false;

		char** componentNameList;
		char** behaviorNameList;

		int componentNameListSize;
		int behaviorNameListSize;

		int currentIndex_componentList = 0;
		int currentIndex_behaviorList = 0;


		
		std::weak_ptr< CBuffer_Dx12< TestGSVariable>> shp_testGSVariable;
		float t = 0;
		int dir = 1;
	};
}
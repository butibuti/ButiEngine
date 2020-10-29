#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class IScene;
	class GameObjectManager :public IObject
	{
		friend class GameObject;
	public:
		GameObjectManager(std::weak_ptr<IScene> arg_wkp_scene);
		void Update();
		void Initialize()override;
		void PreInitialize()override;
		std::weak_ptr<GameObject> AddObject(std::shared_ptr<GameObject> arg_gameObject);
		std::weak_ptr<GameObject> AddObject(std::shared_ptr<Transform> arg_transform,  std::string arg_objectName = "GameObject");
		
		std::weak_ptr<GameObject> GetGameObject(const std::string& arg_objectName);
		std::weak_ptr<GameObject> GetGameObject(const GameObjectTag& arg_objectTag);
		std::vector<std::weak_ptr<GameObject>> GetGameObjects(const GameObjectTag& arg_objectTag);


		std::weak_ptr<IScene> GetScene() {
			return wkp_scene;
		}

	private:
		void UnRegistGameObject(std::shared_ptr<GameObject> arg_gameObject);
		std::vector<std::shared_ptr< GameObject>> vec_gameObjects;
		std::vector<std::shared_ptr< GameObject>> vec_newGameObjects;
		std::map<std::string, std::weak_ptr< GameObject>> map_gameObjects;
		std::weak_ptr<IScene> wkp_scene;
	};

}
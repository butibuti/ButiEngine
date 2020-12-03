#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class IScene;
	class GameObjectManager :public IObject
	{
		friend class GameObject;
	public:
		GameObjectManager(std::weak_ptr<IScene> arg_wkp_scene);
		GameObjectManager(){}
		void SetScene(std::weak_ptr<IScene> arg_wkp_scene);
		void Update();
		void RegistNewGameObject();
		void Initialize()override;
		void PreInitialize()override;
		void ShowUI();
		void Release();
		void Initialize_cereal();

		std::weak_ptr<GameObject> GetSelectedUI();

		std::weak_ptr<GameObject> AddObject(std::shared_ptr<Transform> arg_transform, std::string arg_objectName = "GameObject");
		std::weak_ptr<GameObject> AddObject(std::shared_ptr<GameObject> arg_gameObject);
		std::weak_ptr<GameObject>AddObjectFromCereal(std::string filePath, std::shared_ptr<Transform> arg_transform = nullptr);
		std::weak_ptr<GameObject>AddObjectFromCereal_Insert(std::string filePath, std::shared_ptr<Transform> arg_transform=nullptr);

		std::string ReNameGameObject(const std::string& arg_ObjectName,const std::string& arg_befObjectName);

		std::weak_ptr<GameObject> GetGameObject(const std::string& arg_objectName);
		std::weak_ptr<GameObject> GetGameObject(const GameObjectTag& arg_objectTag);
		std::vector<std::weak_ptr<GameObject>> GetGameObjects(const GameObjectTag& arg_objectTag);

		void Start();

		std::weak_ptr<IScene> GetScene() {
			return wkp_scene;
		}

		std::weak_ptr<IApplication> GetApplication();

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vec_gameObjects);
			archive(map_gameObjects);
		}
	private:
		void UnRegistGameObject(std::shared_ptr<GameObject> arg_gameObject);
		std::vector<std::shared_ptr< GameObject>> vec_gameObjects;
		std::vector<std::shared_ptr< GameObject>> vec_newGameObjects;
		std::unordered_map<std::string, std::weak_ptr< GameObject>> map_gameObjects;
		std::weak_ptr<IScene> wkp_scene;
		std::weak_ptr<GameObject> selectedGameObject;
	};

	void OutputCereal(const std::shared_ptr<GameObjectManager>& v, const std::string& path);

	void InputCereal(std::shared_ptr<GameObjectManager>& v, const std::string& path);
}


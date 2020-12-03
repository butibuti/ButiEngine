#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class GameObject;
	using GameObjectTag = ID<GameObject>;
	class GameComponent;
	class Behavior;
	class GameObjectManager;
	class Application;
	class ResourceContainer;
	class GraphicDevice;
	class GameObject:public IObject
	{
		enum CollisionTiming {
			enter,keep,leave
		};
		friend class Behavior;
		friend class GameComponent;
	public:
		GameObject();

		GameObject(std::shared_ptr<Transform> arg_transform, const std::string& arg_objectName = "GameObject", const std::string& arg_tagName="none");

		void Update();
		void Translate(const Vector3& velocity);
		void ScaleChange(const Vector3& changePase);
		void Rotation(const Vector3& rotationPase);

		void Start();

		void SetActive(bool arg_isActive);

		void SetGameObjectManager(std::weak_ptr<GameObjectManager> arg_wkp_gameObjectManager) {
			wkp_gameObjManager = arg_wkp_gameObjectManager;
		}

		bool GetActive();

		void SetIsRemove(bool arg_isRemove);

		bool GetIsRemove();

		virtual void OnUpdate();

		virtual void Hit(std::shared_ptr< GameObject> shp_other);
		
		virtual void Release();

		virtual void Initialize();

		virtual void PreInitialize();

		std::shared_ptr<Behavior> AddBehavior(std::shared_ptr<Behavior> arg_shp_behavior);

		template<class T, typename... Ts>
		inline std::shared_ptr<T> AddBehavior(Ts&&... params) {
			auto addBehavior = ObjectFactory::Create<T>(params...);
			vec_newBehavior.push_back(addBehavior);
			addBehavior->Set(GetThis<GameObject>());
			return addBehavior;
		}

		template<class T, typename... Ts>
		inline std::shared_ptr<T> AddBehavior_Insert(Ts&&... params) {
			auto addBehavior = ObjectFactory::Create<T>(params...);
			vec_behaviors.push_back(addBehavior);
			addBehavior->Set(GetThis<GameObject>());
			return addBehavior;
		}

		std::shared_ptr<GameComponent> AddGameComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent);

		template<class T, typename... Ts>
		inline std::shared_ptr<T> AddGameComponent(Ts&&... params) {
			auto addComponet = ObjectFactory::Create<T>(params...);
			vec_newGameComponent.push_back(addComponet);
			addComponet->Set(GetThis<GameObject>());
			return addComponet;
		}
		
		template<class T, typename... Ts>
			inline std::shared_ptr<T> AddGameComponent_Insert(Ts&&... params) {
			auto addComponet = ObjectFactory::Create<T>(params...);
			vec_gameComponents.push_back(addComponet);
			addComponet->Set(GetThis<GameObject>());
			return addComponet;
		}

		std::shared_ptr<Behavior> AddBehavior_Insert(std::shared_ptr<Behavior> arg_shp_behavior);

		std::shared_ptr<GameComponent> AddGameComponent_Insert(std::shared_ptr<GameComponent> arg_shp_gameComponent);

		std::shared_ptr<Behavior> GetBehavior(const std::string& arg_behaviorName);

		std::shared_ptr<GameComponent> GetGameComponent(const std::string& arg_gameComponentName);


		template <typename T>
		std::shared_ptr<T> GetGameComponent() {
			std::string arg_gameComponentName = ObjectFactory::Create<T>()->GetGameComponentName();

			auto ret = GetGameComponent(arg_gameComponentName);
			if (!ret) {
				return nullptr;
			}
			return ret->GetThis<T>();
		}

		template <typename T>
		std::shared_ptr<T> GetBehavior() {
			std::string arg_gameComponentName = ObjectFactory::Create<T>()->GetBehaviorName();

			auto ret = GetBehavior(arg_gameComponentName);
			if (!ret) {
				return nullptr;
			}
			return ret->GetThis<T>();
		}

		void RemoveBehavior(const std::string& arg_key);

		void RemoveGameComponent(const std::string& arg_key);

		std::shared_ptr<Transform> transform= ObjectFactory::Create<Transform>();

		std::weak_ptr<GameObject> GetBaseTransform() {
			return parent;
		}

		void ShowUI();

		std::weak_ptr<GameObject> SetParent(std::weak_ptr<GameObject> arg_parent) {
			parent = arg_parent;
			return parent;
		}

		std::weak_ptr<GameObject> AddChildGameObject(std::weak_ptr<GameObject> arg_childGameObject) {
			vec_childGameObjects.push_back(arg_childGameObject);
			return arg_childGameObject;
		}

		std::string GetGameObjectName() const{
			return objectName;
		}

		std::string SetObjectName(const std::string& arg_objectName);

		GameObjectTag SetGameObjectTag(GameObjectTag arg_tag) {
			gameObjectTag = arg_tag;
			return gameObjectTag;
		}

		GameObjectTag GetGameObjectTag()const {
			return gameObjectTag;
		}

		std::weak_ptr<GameObjectManager> GetGameObjectManager();
		std::weak_ptr<IApplication> GetApplication();
		std::shared_ptr<IResourceContainer> GetResourceContainer();
		std::shared_ptr<GraphicDevice> GetGraphicDevice();

		void UpdateTagName();

		std::shared_ptr<GameObject> Clone();

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(transform);
			archive(objectName);
			archive(tagName);

			archive(vec_behaviors);
			archive(vec_gameComponents);
		}
		void Init_RegistGameComponents();
		void Init_RegistBehaviors();


	protected:

		std::shared_ptr<Behavior> RegisterBehavior(std::shared_ptr<Behavior> arg_shp_behavior);

		std::shared_ptr<GameComponent> RegisterGameComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent);

		void GameComponentUpdate();
		void BehaviorUpdate();
		void BehaviorHit();

		std::weak_ptr<GameObject> parent;
		std::vector<std::weak_ptr<GameObject>>vec_childGameObjects;

		bool isActive = true;

		bool isRemove = false;
		
		std::vector< std::shared_ptr< GameComponent>>  vec_gameComponents;
		std::vector< std::shared_ptr< Behavior>>  vec_behaviors;
		std::vector<std::shared_ptr<GameComponent>> vec_newGameComponent;
		std::vector<std::shared_ptr<Behavior>>vec_newBehavior;

		std::string objectName;
		std::string tagName;
		GameObjectTag gameObjectTag;

		std::weak_ptr<GameObjectManager> wkp_gameObjManager;

		std::vector<std::shared_ptr<GameObject>> vec_collisionObject;
		std::vector<std::shared_ptr<GameObject>> vec_befCollisionObject;
	};


	void OutputCereal(const std::shared_ptr<GameObject>& v);

	void InputCereal(std::shared_ptr<GameObject>& v,  const std::string& path);
}
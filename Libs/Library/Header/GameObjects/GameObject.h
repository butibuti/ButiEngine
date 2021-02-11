#pragma once
#include<memory>
#include<vector>
namespace ButiEngine {
	class GameObject;
	using GameObjectTag = ID<GameObject>;
	class GameComponent;
	class GameObjectManager;
	class Application;
	class ResourceContainer;
	class GraphicDevice;
	class GameObject:public IObject
	{
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

		void SetGameObjectManager(std::weak_ptr<GameObjectManager> arg_wkp_gameObjectManager);
		bool GetActive();

		void SetIsRemove(bool arg_isRemove);

		bool GetIsRemove();

		virtual void OnUpdate();

		virtual void Hit(std::shared_ptr< GameObject> shp_other);
		
		virtual void Release();

		virtual void Initialize();

		virtual void PreInitialize();

		void RegistReactionComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent);

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


		std::shared_ptr<GameComponent> AddGameComponent_Insert(std::shared_ptr<GameComponent> arg_shp_gameComponent);

		std::shared_ptr<GameComponent> GetGameComponent(const std::string& arg_gameComponentName,unsigned int index);


		template <typename T>
		std::shared_ptr<T> GetGameComponent(unsigned int index=0) {
			std::string arg_gameComponentName = ObjectFactory::Create<T>()->GetGameComponentName();

			auto ret = GetGameComponent(arg_gameComponentName,index);
			if (!ret) {
				return nullptr;
			}
			return ret->GetThis<T>();
		}


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


		std::shared_ptr<GameObject> Clone();

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(transform);
			archive(objectName);
			archive(gameObjectTag);
			archive(vec_gameComponents);
		}
		void Init_RegistGameComponents();


	protected:
		std::shared_ptr<GameComponent> RegisterGameComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent);

		void GameComponentUpdate();
		void BehaviorHit();

		std::weak_ptr<GameObject> parent;
		std::vector<std::weak_ptr<GameObject>>vec_childGameObjects;

		bool isActive = true;

		bool isRemove = false;
		
		std::vector< std::shared_ptr< GameComponent>>  vec_gameComponents;
		std::vector< std::shared_ptr< GameComponent>>  vec_collisionReactionComponents;
		std::vector<std::shared_ptr<GameComponent>> vec_newGameComponent;

		std::string objectName;
		GameObjectTag gameObjectTag;

		std::weak_ptr<GameObjectManager> wkp_gameObjManager;

		std::vector<std::shared_ptr<GameObject>> vec_collisionObject;
		std::vector<std::shared_ptr<GameObject>> vec_befCollisionObject;
	};


	void OutputCereal(const std::shared_ptr<GameObject>& v);

	void InputCereal(std::shared_ptr<GameObject>& v,  const std::string& path);
}
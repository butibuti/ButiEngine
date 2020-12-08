#pragma once

#include "../GameObjects/Behavior.h"
#include "../GameObjects/GameComponent.h"
namespace ButiEngine {
	class ComponentsLoader :public IObject{
	public:

		static void CreateInstance();
		static void SaveInstance();

		static  std::shared_ptr< ComponentsLoader> GetInstance();

		void Initialize(){}
		void PreInitialize(){}
		void Release();
		void CreateNameList();
		void AddBehavior(std::shared_ptr<Behavior> arg_behavior);
		void AddGameComponent(std::shared_ptr<GameComponent> arg_gamecomponent);

		void RemoveComponent(const std::string& arg_name);
		void RemoveBehavior(const std::string& arg_name);

		std::shared_ptr<Behavior> ShowAddBehaviorUI();
		std::shared_ptr<GameComponent> ShowAddGameComponentUI();

		template <typename T>
		void AddBehavior() {
			AddBehavior(ObjectFactory::Create<T>());
		}
		template <typename T>
		void AddGameComponent() {
				AddGameComponent(ObjectFactory::Create<T>());
		}


		template <typename T>
		void RemoveComponent() {
			RemoveComponent(ObjectFactory::Create<T>()->GetGameComponentName());
		}
		template <typename T>
		void RemoveBehavior() {
			RemoveBehavior( ObjectFactory::Create<T>()->GetBehaviorName());
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(addBehaviors);
			archive(addGameComponents);
		}
	private:
		
		std::vector<std::shared_ptr<Behavior>> addBehaviors;
		std::vector<std::shared_ptr<GameComponent>> addGameComponents;


		char** componentNameList=nullptr;
		char** behaviorNameList=nullptr;

		int componentNameListSize;
		int behaviorNameListSize;

		int currentIndex_componentList = 0;
		int currentIndex_behaviorList = 0;
		static std::shared_ptr< ComponentsLoader> instance;
	};


	void OutputCereal(const std::shared_ptr<ComponentsLoader>& v, const std::string& path);

	void InputCereal(std::shared_ptr<ComponentsLoader>& v, const std::string& path);
}
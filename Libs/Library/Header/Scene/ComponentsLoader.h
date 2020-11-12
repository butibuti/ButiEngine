#pragma once
#include "stdafx.h"
#include "Header/GameObjects/Behavior.h"
#include "Header/GameObjects/GameComponent.h"
namespace ButiEngine {
	class ComponentsLoader :public IObject{
	public:
		ComponentsLoader(){}
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
	};


	void OutputCereal(const std::shared_ptr<ComponentsLoader>& v, const std::string& path);

	void InputCereal(std::shared_ptr<ComponentsLoader>& v, const std::string& path);
}
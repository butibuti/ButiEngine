#pragma once

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
		void AddGameComponent(std::shared_ptr<GameComponent> arg_gamecomponent);

		void RemoveComponent(const std::string& arg_name);

		std::shared_ptr<GameComponent> ShowAddGameComponentUI();

		template <typename T>
		void AddGameComponent() {
				AddGameComponent(ObjectFactory::Create<T>());
		}


		template <typename T>
		void RemoveComponent() {
			RemoveComponent(ObjectFactory::Create<T>()->GetGameComponentName());
		}


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(addGameComponents);
		}
	private:
		
		std::vector<std::shared_ptr<GameComponent>> addGameComponents;


		char** componentNameList=nullptr;

		int componentNameListSize;

		int currentIndex_componentList = 0;
		static std::shared_ptr< ComponentsLoader> instance;
	};


	void OutputCereal(const std::shared_ptr<ComponentsLoader>& v, const std::string& path);

	void InputCereal(std::shared_ptr<ComponentsLoader>& v, const std::string& path);
}
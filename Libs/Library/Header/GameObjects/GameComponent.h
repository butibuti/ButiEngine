#pragma once
#include"stdafx.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionManager;
	}
	class GameComponent :public IObject
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		void Update();
		virtual void Start(){}
		void Set(std::weak_ptr<GameObject> arg_wkp_gameObj);
		void SetIsRemove(bool arg_isRemove) {
			isRemove = arg_isRemove;
		}
		void SetIsActive(bool arg_isActive) {
			isActive = arg_isActive;
		}
		void ShowUI() {

			ImGui::Checkbox("IsActive", &isActive);
			OnShowUI();
		}
		virtual void OnShowUI() {}
		virtual void OnSet();
		virtual void OnRemove();
		virtual std::string GetGameComponentName() = 0;
		virtual std::shared_ptr<GameComponent> Clone()=0;
		bool IsRemove() {
			return isRemove;
		}
		bool IsActive() {
			return isActive;
		}
	protected:
		virtual void OnUpdate() = 0;
		std::weak_ptr<ICamera>& GetCamera();
		std::weak_ptr<ICamera>& GetCamera(const UINT arg_camNum);
		std::weak_ptr<ICamera>& GetCamera(const std::string& arg_camName);
		std::weak_ptr<GameObjectManager> GetManager();
		std::weak_ptr<Collision::CollisionManager> GetCollisionManager();
		std::weak_ptr<GameObject> gameObject;
		bool isActive=true;
		bool isRemove=false;
	};

#define BUTI_REGIST_GAMECOMPONENT(T)\
	CEREAL_REGISTER_TYPE(T);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, T);
}
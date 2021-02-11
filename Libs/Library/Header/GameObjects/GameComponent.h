#pragma once
#include<memory>
namespace ButiEngine {
	class GameComponent :public IObject
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		void Update();
		virtual void Start() {}
		void Set(std::weak_ptr<GameObject> arg_wkp_gameObj);
		void SetIsRemove(bool arg_isRemove) {
			isRemove = arg_isRemove;
		}
		void SetIsActive(bool arg_isActive) {
			isActive = arg_isActive;
		}
		void ShowUI() {

			GUI::Checkbox("IsActive", &isActive);
			OnShowUI();
		}
		virtual void OnShowUI() {}
		virtual void OnSet();
		virtual void OnRemove();
		virtual void OnCollision(std::weak_ptr<GameObject> arg_other){}
		virtual void OnCollisionEnter(std::weak_ptr<GameObject> arg_other){}
		virtual void OnCollisionEnd(std::weak_ptr<GameObject> arg_other){}
		virtual std::string GetGameComponentName() = 0;
		virtual std::shared_ptr<GameComponent> Clone() = 0;
		bool IsRemove() {
			return isRemove;
		}
		bool IsActive() {
			return isActive;
		}
	protected:
		virtual void OnUpdate() {};
		std::weak_ptr<ICamera>& GetCamera();
		std::weak_ptr<ICamera>& GetCamera(const UINT arg_camNum);
		std::weak_ptr<ICamera>& GetCamera(const std::string& arg_camName);
		std::weak_ptr<GameObjectManager> GetManager();
		std::weak_ptr<ICollisionManager> GetCollisionManager();
		std::shared_ptr<GameObjectTagManager> GetTagManager();
		std::weak_ptr<GameObject> gameObject;

		bool isActive = true;
		bool isRemove = false;
	};


}
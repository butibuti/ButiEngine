#pragma once
#include<memory>
namespace ButiEngine {
	class Behavior:public IObject
	{
	public:
		void Update() ;
		void Set(std::weak_ptr<GameObject> arg_wkp_gameObj);
		void SetIsRemove(bool arg_isRemove) {
			isRemove = arg_isRemove;
		}
		void SetIsActive(bool arg_isActive) {
			isActive = arg_isActive;
		}
		void Initialize()override{}
		void PreInitialize()override{}
		virtual void OnSet();
		virtual void OnRemove();
		virtual void Start();
		virtual void OnCollision(std::weak_ptr<GameObject> arg_other);
		virtual void OnCollisionEnter(std::weak_ptr<GameObject> arg_other);
		virtual void OnCollisionEnd(std::weak_ptr<GameObject> arg_other);
		virtual std::shared_ptr<Behavior> Clone() = 0;
		void ShowUI();
		virtual void OnShowUI(){}
		virtual std::string GetBehaviorName() = 0;
		bool IsRemove() {
			return isRemove;
		}
		bool IsActive() {
			return isActive;
		}
	protected:
		virtual void OnUpdate() {};
		std::weak_ptr<GameObject> gameObject;
		std::weak_ptr<ICamera>& GetCamera();
		std::weak_ptr<ICamera>& GetCamera(const UINT arg_camNum);
		std::weak_ptr<ICamera>& GetCamera(const std::string& arg_camName);
		std::weak_ptr<GameObjectManager> GetManager();
		bool isActive=true;
		bool isRemove;
	};

	
}


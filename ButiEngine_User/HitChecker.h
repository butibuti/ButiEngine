#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class HitChecker :public GameComponent
	{
	public:
		HitChecker() {}

		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "HitChecker";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void ShowGUI()override;
	private:

		std::string hitObjName;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::HitChecker)
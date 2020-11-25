#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class Tilter :public GameComponent
	{
	public:

		void OnUpdate()override{}
		void Start()override;
		std::string GetGameComponentName()override {
			return "Tilter";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(angle);
		}
		void OnShowUI();
		void SetAngle(const float angle);
	private:
		float angle;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::Tilter);

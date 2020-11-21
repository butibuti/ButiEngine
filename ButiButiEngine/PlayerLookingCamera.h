#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class PlayerLookingCamera :public GameComponent
	{
	public:
		PlayerLookingCamera() {}

		void OnUpdate()override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "PlayerLookingCamera";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_lookTarget);
			archive(per);
			archive(blendPos);
			archive(isActive);
		}
		void OnShowUI();
	private:
		std::shared_ptr<Transform> shp_lookTarget;
		Vector3 blendPos;
		float per;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::PlayerLookingCamera);
#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class PlayerLookingCamera :public GameComponent
	{
	public:

		void OnUpdate()override;
		void OnSet()override;
		void Detach();
		void SetBlendPos(const Vector3& arg_pos );
		void SetPer(const float per);
		void SetTarget(std::shared_ptr<Transform> arg_shp_lookTarget);
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
		float targetPer;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::PlayerLookingCamera);
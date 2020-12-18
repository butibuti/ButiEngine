#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class PostEffectParamUpdater :public GameComponent
	{
	public:
		PostEffectParamUpdater(const Vector4& pase);
		PostEffectParamUpdater();
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "PostEffectParamUpdater";
		}
		void OnSet();
		void Start();
		std::shared_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(pase);
			archive(isActive);
		}
	private:
		Vector4 pase;
		std::shared_ptr<CBuffer<LightVariable>> shp_param;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::PostEffectParamUpdater)
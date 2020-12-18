#pragma once

#include"../../GameComponentHeader.h"

namespace ButiEngine {

	struct ModelDrawData;
	struct Bone;
	class IKComponent :public GameComponent
	{
	public:
		IKComponent(){}
		std::string GetGameComponentName()override {
			return "IKComponent";
		}

		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;

		void OnShowUI()override;

		void OnUpdate()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}


	protected:

	private:
		std::shared_ptr<ModelDrawData> shp_modelData;
		std::vector<std::shared_ptr<Bone>> vec_endBones;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::IKComponent)

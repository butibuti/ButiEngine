#pragma once

#include"../../GameComponentHeader.h"

namespace ButiEngine {

	struct ModelDrawData;
	struct Bone;
	struct IKData;
	class IKComponent :public GameComponent
	{
	public:
		IKComponent(){}
		std::string GetGameComponentName()override {
			return "IKComponent";
		}
		void OnRemove()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;

		void OnShowUI()override;

		void OnUpdate()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(vec_ikData);
		}


	protected:
		void RegistIK();
	private:
		std::shared_ptr<ModelDrawData> shp_modelData;
		std::vector<std::shared_ptr<Bone>> vec_endBones;
		std::vector<std::shared_ptr< IKData>> vec_ikData;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::IKComponent)

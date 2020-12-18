#pragma once
#include"MeshDrawComponent.h"

namespace ButiEngine {
	class ModelAnimation;
	struct ModelDrawData;
	class ModelDrawComponent :public MeshDrawComponent
	{
	public:
		ModelDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo, const UINT arg_layer=0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		ModelDrawComponent(){}
		std::string GetGameComponentName() {
			return "ModelDraw";
		}

		void OnSet()override;
		std::shared_ptr<ModelDrawData> GetModelData();
		std::shared_ptr<GameComponent> Clone()override;

		void OnShowUI()override;


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(layer);
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
		}


	protected:
		void CreateData()override;
	private:
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::ModelDrawComponent)

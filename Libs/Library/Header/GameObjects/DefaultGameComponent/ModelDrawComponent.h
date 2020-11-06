#pragma once
#include"stdafx.h"
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
		void OnUpdate()override;

		void OnSet()override;
		std::shared_ptr<ModelDrawData> GetModelData();
		std::shared_ptr<GameComponent> Clone()override;

	private:
	};

}

CEREAL_REGISTER_TYPE(ButiEngine::ModelDrawComponent);

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, ButiEngine::ModelDrawComponent)
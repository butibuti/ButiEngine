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
		std::string GetGameComponentName() {
			return "ModelDraw";
		}
		void OnUpdate()override;

		void OnSet()override;
		std::shared_ptr<ModelDrawData> GetModelData();
	private:
		std::shared_ptr< DrawInformation >shp_drawInfo;
	};

}
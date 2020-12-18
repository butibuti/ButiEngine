#include "MeshDrawComponent.h"




namespace ButiEngine {


	class MeshDrawComponent_Static :public MeshDrawComponent
	{
	public:
		MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent_Static(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent_Static() {}
		std::string GetGameComponentName() {
			return "MeshDraw_Static";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;
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
	};
}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::MeshDrawComponent_Static)
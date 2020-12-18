#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\MeshDrawComponent_Static.h"

ButiEngine::MeshDrawComponent_Static::MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
	:MeshDrawComponent(arg_meshTag,arg_shaderTag,arg_materialTag,arg_shp_drawInfo,arg_layer,arg_shp_transform)
{
}

ButiEngine::MeshDrawComponent_Static::MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector<MaterialTag>& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
	:MeshDrawComponent(arg_meshTag,arg_shaderTag,arg_materialTag,arg_shp_drawInfo,arg_layer,arg_shp_transform)
{
}

ButiEngine::MeshDrawComponent_Static::MeshDrawComponent_Static(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
	:MeshDrawComponent(arg_modelTag,arg_shaderTag,arg_shp_drawInfo,arg_layer,arg_shp_transform)
{
}

void ButiEngine::MeshDrawComponent_Static::OnUpdate()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MeshDrawComponent_Static::Clone()
{
	if (shp_drawInfo) {
		if (modelTag.IsEmpty()) {
			return ObjectFactory::Create<MeshDrawComponent_Static>(meshTag, shaderTag, materialTag, shp_drawInfo->Clone(), layer, nullptr);
		}
		else {
			return ObjectFactory::Create<MeshDrawComponent_Static>(modelTag, shaderTag, shp_drawInfo->Clone(), layer, nullptr);
		}
	}
	else {

		if (modelTag.IsEmpty()) {
			return ObjectFactory::Create<MeshDrawComponent_Static>(meshTag, shaderTag, materialTag, nullptr, layer, nullptr);
		}
		else {
			return ObjectFactory::Create<MeshDrawComponent_Static>(modelTag, shaderTag, nullptr, layer, nullptr);
		}
	}
}

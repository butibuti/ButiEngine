#pragma once
#include"stdafx.h"
#include"Header/Resources/DrawData/DrawData_Dx12.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\MeshDrawComponent.h"





ButiEngine::MeshDrawComponent::MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	meshTag = arg_meshTag;
	shaderTag = arg_shaderTag;
	materialTag.push_back(arg_materialTag);

	shp_transform = arg_shp_transform;
	shp_drawInfo = arg_shp_drawInfo;
}

ButiEngine::MeshDrawComponent::MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector<MaterialTag>& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	meshTag = arg_meshTag;
	shaderTag = arg_shaderTag;
	materialTag = (arg_materialTag);
	shp_drawInfo = arg_shp_drawInfo;
	if (arg_shp_transform) {
		shp_transform = arg_shp_transform;
	}
}

ButiEngine::MeshDrawComponent::MeshDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	modelTag = arg_modelTag;
	shaderTag = arg_shaderTag;
	shp_drawInfo = arg_shp_drawInfo;
	if (arg_shp_transform) {
		shp_transform = arg_shp_transform;
	}
}

void ButiEngine::MeshDrawComponent::OnUpdate()
{
#ifdef DEBUG
	if (GameDevice::input.TriggerKey(Keys::One)) {
		SwitchFillMode(false);
	}
#endif
}

void ButiEngine::MeshDrawComponent::ChangeFillMode(const bool arg_isFill)
{
	data->ChangeFillMode(arg_isFill);
}

void ButiEngine::MeshDrawComponent::SwitchFillMode(const bool arg_isFill)
{
	data->ChangeSwitchFillMode();
}

void ButiEngine::MeshDrawComponent::ChangeCullMode(const CullMode& arg_cullMode)
{
	data->ChangeCullMode(arg_cullMode);
}

void ButiEngine::MeshDrawComponent::OnSet()
{
	if (!shp_drawInfo) {
		shp_drawInfo = ObjectFactory::Create<DrawInformation>();
	}
	auto renderer = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer();
	if (!shp_transform) {
		shp_transform = gameObject.lock()->transform;
	}
	if (!modelTag.IsEmpty()) {												 
			auto graphicDevice = gameObject.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();
			data = ObjectFactory::Create<MeshDrawData_Dx12>(modelTag, shaderTag,renderer,graphicDevice,shp_drawInfo,shp_transform );

	}
	else {
			data = ObjectFactory::Create<MeshDrawData_Dx12>(meshTag, shaderTag, materialTag, renderer, gameObject.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>(), shp_drawInfo,shp_transform);

		
	}

	index = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->RegistDrawObject(data,layer);
}

void ButiEngine::MeshDrawComponent::OnRemove()
{
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->UnRegistDrawObject(index,layer);
}

void ButiEngine::MeshDrawComponent::SetBlendMode(const BlendMode& arg_blendMode)
{
	data->SetBlendMode(arg_blendMode);
}
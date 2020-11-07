#pragma once
#include"stdafx.h"
#include"Header/Resources/DrawData/DrawData_Dx12.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\MeshDrawComponent.h"
#include "..\..\..\Header\GameParts\ResourceContainer.h"


ButiEngine::MeshDrawComponent::MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	meshTag = arg_meshTag;
	shaderTag = arg_shaderTag;
	materialTag.push_back(arg_materialTag);

	shp_transform = arg_shp_transform;
	shp_drawInfo = arg_shp_drawInfo;
	isCereal = false;
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
	isCereal = false;
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
	isCereal = false;
}

void ButiEngine::MeshDrawComponent::OnUpdate()
{
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

	if (!isCereal)
	{
		auto lightBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<LightVariable>>(2);

		shp_drawInfo->vec_exCBuffer.push_back(lightBuffer_Dx12);

		auto light = LightVariable();
		light.lightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);
		lightBuffer_Dx12->SetExName("LightBuffer");
		lightBuffer_Dx12->Get() = light;
	}
	else {
		auto endItr = shp_drawInfo->vec_exCBuffer.end();
		for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != endItr; itr++) {
			(*itr)->Initialize();
		}
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
	if (index)
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->UnRegistDrawObject(index,layer);
}

void ButiEngine::MeshDrawComponent::SetBlendMode(const BlendMode& arg_blendMode)
{
	data->SetBlendMode(arg_blendMode);
}

std::shared_ptr<ButiEngine:: GameComponent> ButiEngine::MeshDrawComponent::Clone()
{
	if (modelTag.IsEmpty()) {
		return ObjectFactory::Create<MeshDrawComponent>(meshTag, shaderTag,materialTag, shp_drawInfo, layer, shp_transform);
	}
	else {
		return ObjectFactory::Create<MeshDrawComponent>(modelTag,shaderTag,shp_drawInfo,layer,shp_transform);
	}

}

void ButiEngine::MeshDrawComponent::Regist()
{

	if (!index)
	index = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->RegistDrawObject(data, layer);
}

void ButiEngine::MeshDrawComponent::UnRegist()
{
	if(index)
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->UnRegistDrawObject(index, layer);
}

void ButiEngine::MeshDrawComponent::OnShowUI()
{
	auto endItr = shp_drawInfo->vec_exCBuffer.end();
	for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != endItr; itr++) {
		(*itr)->ShowUI();
	}
}

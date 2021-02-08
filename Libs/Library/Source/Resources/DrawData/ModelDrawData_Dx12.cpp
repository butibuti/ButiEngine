#pragma once
#include"stdafx.h"
#include"Header/Device/PipelineStateManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
#include"Header/Resources/DrawData/ModelDrawData_Dx12.h"
#include"Header/GameParts/ResourceContainer.h"


ButiEngine::ModelDrawData_Dx12::ModelDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform>arg_shp_transform)
{
	wkp_graphicDevice = arg_wkp_graphicDevice;
	shp_renderer = arg_shp_renderer;
	modelTag = arg_model;
	vec_bone = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetBone();
	SetIKBone();
	SetAddBone();
	meshTag = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetMeshTag();
	vec_materialTags = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetMaterialTags();
	shaderTag = arg_shader;
	subset = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetSubset();
	shp_drawInfo = arg_shp_drawInfo;
	transform = arg_shp_transform->GetMatrix();
	shp_transform = arg_shp_transform;
}

void ButiEngine::ModelDrawData_Dx12::Initialize()
{
	

	cbuffer_bone = GetCArrayBuffer<Matrix4x4>("bone");

	//ModelUpdate();

	commandList = CommandListHelper::CreateBundleCommandList(wkp_graphicDevice.lock());
	CommandListHelper::Close(commandList);



	int srvCount = 0;
	auto container = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer();
	for (int i = 0; i < vec_materialTags.size(); i++) {
		auto textureCount = container->GetMaterial(vec_materialTags[i]).lock()->GetTextureCount();
		srvCount = max(srvCount, textureCount);
	}

	textureRegion = srvCount;
	DrawData_Dx12::Initialize(srvCount);


}

void ButiEngine::ModelDrawData_Dx12::DrawBefore()
{
	shp_worldMatrixUpdater->WorldMatrixUpdate();
	ModelUpdate();
}


void ButiEngine::ModelDrawData_Dx12::Draw()
{
	BufferUpdate();
	CommandExecute();
}


void ButiEngine::ModelDrawData_Dx12::ModelUpdate()
{
	int i = 0;
	for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++, i++) {
		cbuffer_bone->Get(i) = (*itr)->GetPoseMatrix();
	}
}

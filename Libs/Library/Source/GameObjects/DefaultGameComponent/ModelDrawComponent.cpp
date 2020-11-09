#include "stdafx.h"
#include"Header/Resources/DrawData/ModelDrawData_Dx12.h"
#include"Header/Resources/ModelAnimation.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"
#include "Header/GameParts/ResourceContainer.h"


ButiEngine::ModelDrawComponent::ModelDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo, const UINT arg_layer,  std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	modelTag = arg_modelTag;
	shaderTag = arg_shaderTag;
	shp_drawInfo= arg_shp_drawInfo;
	shp_transform = arg_shp_transform;
	
	isCereal = false;

}

void ButiEngine::ModelDrawComponent::OnUpdate()
{
#ifdef DEBUG
	if (GameDevice::input.TriggerKey(Keys::One)) {
		SwitchFillMode(false);
	}
#endif
}
void ButiEngine::ModelDrawComponent::OnSet()
{

	if (!shp_drawInfo) {
		shp_drawInfo = ObjectFactory::Create<DrawInformation>();
		
	}
	if (!isCereal) {
		{
			auto lightBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<LightVariable>>(2);

			shp_drawInfo->vec_exCBuffer.push_back(lightBuffer_Dx12);

			auto light = LightVariable();
			light.lightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);

			lightBuffer_Dx12->Get() = light;
		}

		auto cb_bone_Dx12 = ObjectFactory::Create<CArrayBuffer_Dx12<Matrix4x4>>(3, gameObject.lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetBone().size());
		cb_bone_Dx12->SetExName("bone");
		shp_drawInfo->vec_exCBuffer.push_back(cb_bone_Dx12);
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

	auto modelData = ObjectFactory::Create<ModelDrawData_Dx12>(modelTag, shaderTag,renderer, gameObject.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>(),shp_drawInfo,shp_transform);

	data = modelData;
	
	for (auto itr =modelData-> vec_bone.begin(); itr != modelData->vec_bone.end(); itr++) {
		if (!((*itr)->parentBone)) {

			(*itr)->transform->SetBaseTransform(shp_transform, true);
		}
	}
	index = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->RegistDrawObject(data,layer);
}

std::shared_ptr<ButiEngine::ModelDrawData> ButiEngine::ModelDrawComponent::GetModelData()
{
	return data->GetThis<ModelDrawData>();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ModelDrawComponent::Clone()
{
	if (shp_drawInfo)
		return ObjectFactory::Create<ModelDrawComponent>(modelTag, shaderTag, shp_drawInfo->Clone(), layer, nullptr);
	else
		return ObjectFactory::Create<ModelDrawComponent>(modelTag, shaderTag, nullptr, layer, nullptr);
}

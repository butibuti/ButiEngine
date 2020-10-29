#include "stdafx.h"
#include"Header/Resources/DrawData/ModelDrawData_Dx12.h"
#include"Header/Resources/ModelAnimation.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"


ButiEngine::ModelDrawComponent::ModelDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo, const UINT arg_layer,  std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	modelTag = arg_modelTag;
	shaderTag = arg_shaderTag;
	shp_drawInfo= arg_shp_drawInfo;
	shp_transform = arg_shp_transform;
	

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
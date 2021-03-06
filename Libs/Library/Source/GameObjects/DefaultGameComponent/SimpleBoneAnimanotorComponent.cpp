#include"stdafx.h"
#include"Header/Resources/ModelAnimation.h"
#include"Header/Resources/DrawData/IDrawData.h"

ButiEngine::SimpleBoneAnimatorComponent::SimpleBoneAnimatorComponent(std::shared_ptr<ModelDrawData> arg_shp_modelData)
{
	shp_modelData = arg_shp_modelData;
}

void ButiEngine::SimpleBoneAnimatorComponent::OnUpdate()
{
	shp_animation->Motion();
	shp_animation->IKTest();
}


void ButiEngine::SimpleBoneAnimatorComponent::AddAnimation(std::shared_ptr<ModelAnimation> arg_shp_animation)
{
	arg_shp_animation->SetBoneDrawObj(shp_modelData);
	shp_animation = arg_shp_animation;
	shp_animation->PreMotionStart(shp_modelData->SerchBoneByIndex(0)->transform);
}

void ButiEngine::SimpleBoneAnimatorComponent::AddAnimation(MotionTag arg_motionTag)
{
	auto shp_animation_add = gameObject.lock()-> GetResourceContainer()->GetMotion(arg_motionTag);
	shp_animation_add->SetBoneDrawObj(shp_modelData);
	shp_animation = shp_animation_add;
	shp_animation->PreMotionStart(shp_modelData->SerchBoneByIndex(0)->transform);
}

void ButiEngine::SimpleBoneAnimatorComponent::SetLoop(const bool arg_isLoop)
{
	shp_animation->SetLoop(arg_isLoop);
}

void ButiEngine::SimpleBoneAnimatorComponent::OnShowUI()
{

}

std::shared_ptr< ButiEngine::GameComponent> ButiEngine::SimpleBoneAnimatorComponent::Clone()
{
	return ObjectFactory::Create<SimpleBoneAnimatorComponent>(shp_modelData);
}

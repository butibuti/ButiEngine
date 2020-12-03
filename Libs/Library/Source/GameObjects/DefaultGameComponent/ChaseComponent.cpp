#include "stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ChaseComponent.h"


BUTI_REGIST_GAMECOMPONENT(ButiEngine::ChaseComponent);

ButiEngine::ChaseComponent::ChaseComponent(std::shared_ptr<Transform> arg_shp_target, const float arg_speed)
{
	shp_target = arg_shp_target;
	speed = arg_speed;
}

void ButiEngine::ChaseComponent::OnUpdate()
{
	if (!shp_target) { return; }
	auto velocity = Vector3(shp_target->GetWorldPosition() - gameObject.lock()->transform->GetWorldPosition());
	gameObject.lock()->Translate (velocity.GetNormalize() * (velocity.GetLength() * velocity.GetLength() )*speed*0.01f);

}

void ButiEngine::ChaseComponent::OnSet()
{
}

void ButiEngine::ChaseComponent::OnShowUI()
{
	GUI::SliderFloat("speed", &speed, 0.0, 1.0, "%.3f");
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ChaseComponent::Clone()
{
	return ObjectFactory::Create<ChaseComponent>(shp_target, speed);
}

#include "stdafx.h"
#include "ChaseComponent.h"
ButiEngine::ChaseComponent::ChaseComponent(std::shared_ptr<Transform> arg_shp_target, const float arg_speed)
{
	shp_target = arg_shp_target;
	speed = arg_speed;
}

void ButiEngine::ChaseComponent::OnUpdate()
{
	auto velocity = Vector3(shp_target->GetWorldPosition() - gameObject.lock()->transform->GetWorldPosition());
	gameObject.lock()->Translate (velocity.GetNormalize() * (velocity.GetLength() * velocity.GetLength() )*speed);

}

void ButiEngine::ChaseComponent::OnSet()
{
}

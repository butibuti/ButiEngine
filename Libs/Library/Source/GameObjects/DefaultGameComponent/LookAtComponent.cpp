#include "stdafx.h"
#include "Header/GameObjects/DefaultGameComponent/LookAtComponent.h"

ButiEngine::LookAtComponent::LookAtComponent(std::shared_ptr<Transform> arg_shp_lookTarget)
{
	shp_lookTarget = arg_shp_lookTarget;
}

void ButiEngine::LookAtComponent::OnUpdate()
{

	if (!shp_lookTarget) { return; }
	gameObject.lock()->transform->SetLookAtRotation(shp_lookTarget->GetWorldPosition());
}

void ButiEngine::LookAtComponent::OnSet()
{
	if (!shp_lookTarget) {
		shp_lookTarget = ObjectFactory::Create<Transform>();
	}
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::LookAtComponent::Clone()
{
	return ObjectFactory::Create<LookAtComponent>(shp_lookTarget);
}

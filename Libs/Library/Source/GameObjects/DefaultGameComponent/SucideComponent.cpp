#include "stdafx.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"

ButiEngine::SucideComponent::SucideComponent(const float arg_float)
{
	count = arg_float;
	shp_timer = ObjectFactory::Create<RelativeTimer>(arg_float);
	shp_timer->Start();
}

void ButiEngine::SucideComponent::OnUpdate()
{
	if (shp_timer->Update()) {
		gameObject.lock()->SetIsRemove(true);
	}
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SucideComponent::Clone()
{
	return ObjectFactory::Create<SucideComponent>(count);
}

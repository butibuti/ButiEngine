#include "stdafx.h"
#include "SucideComponent.h"

ButiEngine::SucideComponent::SucideComponent(const float arg_float)
{
	shp_timer = ObjectFactory::Create<RelativeTimer>(arg_float);
	shp_timer->Start();
}

void ButiEngine::SucideComponent::OnUpdate()
{
	if (shp_timer->Update()) {
		gameObject.lock()->SetIsRemove(true);
	}
}

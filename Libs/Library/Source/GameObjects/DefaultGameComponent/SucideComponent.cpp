#include "stdafx.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\SucideComponent.h"

ButiEngine::SucideComponent::SucideComponent(const float arg_float)
{
	count = arg_float;
	shp_timer = ObjectFactory::Create<RelativeTimer>(arg_float);
	shp_timer->Start();
}

ButiEngine::SucideComponent::SucideComponent()
{

	count = 60.0f;
	shp_timer = ObjectFactory::Create<RelativeTimer>(60.0f);
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
	auto ret = ObjectFactory::Create<SucideComponent>(count);
	return ret;
}

void ButiEngine::SucideComponent::OnShowUI()
{
	shp_timer->ShowGUI();
}

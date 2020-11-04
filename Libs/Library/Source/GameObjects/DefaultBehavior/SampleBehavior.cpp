#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"
void ButiEngine::SampleBehavior::OnUpdate()
{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(1.0f);
		gameObject.lock()->transform->RollLocalRotationX_Degrees(-1.0f);
		gameObject.lock()->transform->RollLocalRotationZ_Degrees(-1.0f);

}

void ButiEngine::SampleBehavior::OnSet()
{
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::SampleBehavior::Clone()
{
	return ObjectFactory::Create<SampleBehavior>();
}

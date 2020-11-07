#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"
void ButiEngine::SampleBehavior::OnUpdate()
{
	t += 0.01f;

	if (t > 1.0f) {
		t = 0.0f;
	}
	auto pos = spline.GetPoint(t);
	gameObject.lock()->transform->SetLocalPosition(pos);
}

void ButiEngine::SampleBehavior::OnSet()
{
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::SampleBehavior::Clone()
{
	return ObjectFactory::Create<SampleBehavior>();
}

void ButiEngine::SampleBehavior::OnShowUI()
{
	ImGui::SliderFloat("Time", &t, 0, 1.0f);
}

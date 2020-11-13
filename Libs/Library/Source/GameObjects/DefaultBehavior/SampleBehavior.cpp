#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::SampleBehavior::OnUpdate()
{
	PositionSet();

	t += 0.0025f;

	if (t > 1.0f) {
	t = 0;
	}


}

void ButiEngine::SampleBehavior::OnSet()
{
}

void ButiEngine::SampleBehavior::Start()
{
	//splineCurve = SplineCurve(Vector3(0,6,-10), { Vector3(-10,5 , -1),Vector3(-5, 2,10),Vector3(5, 3, 5)});
	

}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::SampleBehavior::Clone()
{
	return ObjectFactory::Create<SampleBehavior>();
}

void ButiEngine::SampleBehavior::OnShowUI()
{
	ImGui::BulletText("Time");
	if (ImGui::SliderFloat("##t", &t, 0, 1)) {
		PositionSet();
	}
	ImGui::BulletText("Speed");
	ImGui::DragFloat("##speed",&speed,0,1);
	if (splineCurve.ShowUI()) {
		PositionSet();
	}
}

void ButiEngine::SampleBehavior::PositionSet()
{

	gameObject.lock()->transform->SetLocalPosition(splineCurve.GetPoint(t));
}


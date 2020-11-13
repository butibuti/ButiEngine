#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\SplineCurveMover.h"

void ButiEngine::SplineCurveMover::OnUpdate()
{
	PositionSet();

	t += 0.0025f;

	if (t > 1.0f) {
		t = 0;
	}

}


std::shared_ptr< ButiEngine::GameComponent> ButiEngine::SplineCurveMover::Clone()
{
	auto ret = ObjectFactory::Create<SplineCurveMover>();
	ret->SetSplineCurve(splineCurve);

	return ret;
}

void ButiEngine::SplineCurveMover::OnShowUI()
{
	ImGui::BulletText("Time");
	if (ImGui::SliderFloat("##t", &t, 0, 1)) {
		PositionSet();
	}
	ImGui::BulletText("Speed");
	ImGui::DragFloat("##speed", &speed, 0, 1);
	if (splineCurve.ShowUI()) {
		PositionSet();
	}
}

void ButiEngine::SplineCurveMover::PositionSet()
{
	gameObject.lock()->transform->SetLocalPosition(splineCurve.GetPoint(t));
}

void ButiEngine::SplineCurveMover::SetSplineCurve(const SplineCurve& arg_curve)
{
	splineCurve = arg_curve;
}

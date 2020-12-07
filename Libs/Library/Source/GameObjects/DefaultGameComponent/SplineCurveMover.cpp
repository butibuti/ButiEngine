#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\SplineCurveMover.h"

void ButiEngine::SplineCurveMover::OnUpdate()
{
	PositionSet();

	t +=speed;

	if (t > 1.0f) {
		if(splineCurve.IsLoop())
		t = 0;
		else {
			SetIsRemove(true);
		}
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
	GUI::BulletText("Time");
	if (GUI::SliderFloat("##t", &t, 0, 1)) {
		PositionSet();
	}
	GUI::BulletText("Speed");
	GUI::DragFloat("##speed", &speed, 0, 1);
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

void ButiEngine::SplineCurveMover::SetSpeed(const float arg_spped)
{
	speed = arg_spped;
}

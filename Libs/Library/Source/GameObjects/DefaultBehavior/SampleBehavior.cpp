#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/TransformAnimation.h"

void ButiEngine::SampleBehavior::OnUpdate()
{

	if ( t %20==0)
	{
		auto add = gameObject.lock()->Clone();

		auto anim = add->GetGameComponent<TransformAnimation>();
		if(anim)
		add->GetGameComponent<TransformAnimation>()->SetIsActive(false);

		auto sample= add->GetBehavior<SampleBehavior>();

		if (sample)
			sample->SetIsActive(false);

		gameObject.lock()->GetGameObjectManager().lock()->AddObject(add);
		add->Init_RegistBehaviors();
		add->Init_RegistGameComponents();
	}

	if (t < 301)
		t += 1;
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
}

void ButiEngine::SampleBehavior::PositionSet()
{

}


#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"

#include "..\..\Header\Common\CerealUtill.h"
BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);
void ButiEngine::SampleBehavior::OnUpdate()
{
	if (GameDevice::GetInput()->CheckKey(Keys::W)) {
		gameObject.lock()->transform->TranslateZ(0.1f);
	}
	if (GameDevice::GetInput()->CheckKey(Keys::S)) {
		gameObject.lock()->transform->TranslateZ(-0.1f);
	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		gameObject.lock()->transform->TranslateX(0.1f);
	}
	if (GameDevice::GetInput()->CheckKey(Keys::A)) {
		gameObject.lock()->transform->TranslateX(-0.1f);
	}
	if (GameDevice::GetInput()->CheckKey(Keys::LeftShift)) {
		gameObject.lock()->transform->TranslateY(-0.1f);
	}
	if (GameDevice::GetInput()->CheckKey(Keys::Space)) {
		gameObject.lock()->transform->TranslateY(0.1f);
	}

	//shp_mesh->Update();
	pos *= onenDegree;

	//gameObject.lock()->transform->SetLocalPosition(Vector3( pos.real(),pos.imag(),0));
}

void ButiEngine::SampleBehavior::OnSet()
{
}

void ButiEngine::SampleBehavior::Start()
{
	onenDegree = std::polar(1.0f, DirectX::XM_2PI / 2 / 180);
	pos = std::complex<float>(4, 3);
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::SampleBehavior::Clone()
{
	return ObjectFactory::Create<SampleBehavior>();
}

void ButiEngine::SampleBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	GUI::Begin("##Collision");
	GUI::Text("Enter");
	GUI::End();
}

void ButiEngine::SampleBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
	GUI::Begin("##Collision");

	GUI::Text(("OnCollison"+arg_other.lock()->GetGameObjectName()+"*"+gameObject.lock()->GetGameObjectName()).c_str());
	GUI::End();
}

void ButiEngine::SampleBehavior::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
	GUI::Begin("##Collision");

	GUI::Text("End");
	GUI::End();
}

void ButiEngine::SampleBehavior::OnShowUI()
{
}

void ButiEngine::SampleBehavior::PositionSet()
{

}


#include "stdafx.h"
#include "..\include\PlayerBehavior.h"

void ButiEngine::PlayerBehavior::Start()
{
	initPos = gameObject.lock()->transform->GetLocalPosition();
	initRotate = gameObject.lock()->transform->GetLocalRotation();
}

void ButiEngine::PlayerBehavior::OnUpdate()
{
	if (GameDevice::input.CheckKey(Keys::A)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-controllPase);
	}
	else if (GameDevice::input.CheckKey(Keys::D)) {

		gameObject.lock()->transform->RollLocalRotationY_Degrees(controllPase);
	}

	Vector3 velocity = gameObject.lock()->transform->GetFront();
	
	velocity *= speed;
	inertia *= inertiaMinorPase;
	velocity += inertia;

	gameObject.lock()->transform->Translate(velocity);

	inertia += velocity * 0.1f;

	if (gameObject.lock()->transform->GetLocalPosition().z > 50) {
		gameObject.lock()->transform->SetLocalPosition(initPos);
		gameObject.lock()->transform->SetLocalRotation(initRotate);
	}
}

void ButiEngine::PlayerBehavior::OnShowUI()
{
	ImGui::BulletText("Speed");
	if (ImGui::SliderFloat("##speed", &speed, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("Air resistance");
	if (ImGui::SliderFloat("##inertiaMinorPase", &inertiaMinorPase, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("ControllPase");
	if (ImGui::SliderFloat("##controllPase", &controllPase, 0.0f, 10.0f, "%.3f", 1.0f)) {
	}
}

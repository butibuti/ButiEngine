#include "stdafx.h"
#include "..\include\PlayerBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"

void ButiEngine::PlayerBehavior::Start()
{
	initPos = gameObject.lock()->transform->GetLocalPosition();
	initRotate = gameObject.lock()->transform->GetLocalRotation();
	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::CreateGameObjectTag("Player"));
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->ActiveCollision(true);
	
	auto bomb = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("bomb");
	bomb.lock()->transform->SetBaseTransform( gameObject.lock()->transform,true);
}

void ButiEngine::PlayerBehavior::OnUpdate()
{
	moveForce *= 0.7f;
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

	gameObject.lock()->transform->Translate(velocity+moveForce);

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

void ButiEngine::PlayerBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{

	if (arg_other.lock()->GetGameObjectTag() != GameObjectTagManager::GetObjectTag("Enemy")) {
		return;
	}
	moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition() - arg_other.lock()->transform->GetWorldPosition())).GetNormalize() * 2.5f;
	hp--;
	if (hp <= 0) {
		controllPase=0;
		speed = 0;
		gameObject.lock()->AddGameComponent<SucideComponent>(30.0f);
	}
}

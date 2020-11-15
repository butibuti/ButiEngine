#include "stdafx.h"
#include "..\include\EnemyBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"

void ButiEngine::EnemyBehavior::Start()
{
	gameObject.lock()->SetGameObjectTag( GameObjectTagManager::GetObjectTag("Enemy"));
	
}

void ButiEngine::EnemyBehavior::OnUpdate()
{
	moveForce*= 0.7f;
	gameObject.lock()->transform->Translate(velocity*speed+moveForce);
}

void ButiEngine::EnemyBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag()==GameObjectTagManager::GetObjectTag("Bomb")) {
		hp--;
		velocity = Vector3();
		moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition()-arg_other.lock()->transform->GetWorldPosition() )).GetNormalize()*2.0f;
		if (hp <= 0) {
			gameObject.lock()->AddGameComponent<SucideComponent>(60.0f);
		}
	}
}

void ButiEngine::EnemyBehavior::OnShowUI()
{
	ImGui::BulletText("HP");
	int refHp = hp;
	ImGui::DragInt("##hp",&refHp);
	hp = refHp;
	ImGui::BulletText("Velocity");

	if (ImGui::DragFloat3("##velocity", &velocity.x, 0.01f, -10, 10, "%.3f")) {
		velocity.Normalize();
	}
	ImGui::BulletText("Speed");

	ImGui::DragFloat("##speed", &speed, 0.01f,-1, 1);
}

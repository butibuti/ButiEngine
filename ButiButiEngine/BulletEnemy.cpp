#include "stdafx.h"
#include "BulletEnemy.h"
#include "Bullet.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include"include/GameController.h"

void ButiEngine::BulletEnemy::Start()
{
	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::GetObjectTag("Enemy"));


	controller = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("GameController").lock()->GetGameComponent<GameController>();

	stagemax = controller->GetStageMax();
	stagemin = controller->GetStageMin();
}

void ButiEngine::BulletEnemy::OnSet()
{
	if (!shp_timer) {
		shp_timer = ObjectFactory::Create<RelativeTimer>(60);
	}
}

void ButiEngine::BulletEnemy::OnUpdate()
{
	moveForce *= 0.7f;
	gameObject.lock()->transform->Translate(velocity * speed + moveForce);

	auto pos = gameObject.lock()->transform->GetWorldPosition();
	if (pos.x < stagemin.x) {
		pos.x = stagemin.x;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}
	else if (pos.x > stagemax.x) {
		pos.x = stagemax.x;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}

	if (pos.z > stagemax.z) {
		pos.z = stagemax.z;
		gameObject.lock()->SetIsRemove(true);
	}
	else if (pos.z < stagemin.z - 5) {
		pos.z = stagemin.z;
		gameObject.lock()->SetIsRemove(true);
	}
	if (shp_timer->Update()) {
		auto bulletVeloc =(Vector3) (gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player").lock()->transform->GetWorldPosition()-gameObject.lock()->transform->GetWorldPosition());
		bulletVeloc.Normalize();

		auto bullet = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("Bullet").lock();
		auto bulletBehavior = bullet->GetBehavior<Bullet>();
		bullet->transform->SetWorldPosition(gameObject.lock()->transform->GetWorldPosition());
		bulletBehavior->SetVelocity(bulletVeloc);
		bulletBehavior->SetSpeed(0.3);
	}
}

void ButiEngine::BulletEnemy::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag() == GameObjectTagManager::GetObjectTag("Bomb")) {
		hp--;
		speed = 0.025;
		//velocity = Vector3();
		//moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition()-arg_other.lock()->transform->GetWorldPosition() )).GetNormalize()*2.0f;
		if (hp <= 0) {
			controller->AddScore(score);
			gameObject.lock()->SetIsRemove(true);
		}
	}
}

void ButiEngine::BulletEnemy::OnShowUI()
{
	ImGui::BulletText("HP");
	int refHp = hp;
	ImGui::DragInt("##hp", &refHp);
	hp = refHp;
	ImGui::BulletText("Velocity");

	if (ImGui::DragFloat3("##velocity", &velocity.x, 0.01f, -10, 10, "%.3f")) {
		velocity.Normalize();
	}
	ImGui::BulletText("Speed");

	ImGui::DragFloat("##speed", &speed, 0.01f, -1, 1);

	ImGui::BulletText("Score");
	ImGui::DragInt("##score", &score, 1, 0, 100);
	shp_timer->ShowGUI();
}

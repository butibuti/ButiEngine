#include "stdafx.h"
#include"BossEnemy.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include"include/GameController.h"
#include"Header/GameObjects/DefaultGameComponent/CameraMan.h"

void ButiEngine::BossEnemy::Start()
{
	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::GetObjectTag("Enemy"));


	controller = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("GameController").lock()->GetGameComponent<GameController>();

	stagemax = controller->GetStageMax();
	stagemin = controller->GetStageMin();
}

void ButiEngine::BossEnemy::OnUpdate()
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
	if (pos.z < stagemin.z - 20) {
		pos.z = stagemin.z;
		gameObject.lock()->SetIsRemove(true);

		controller->DestroyBoss(0);
	}
}

void ButiEngine::BossEnemy::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag() == GameObjectTagManager::GetObjectTag("Bomb")) {

		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Slash.wav", "Sound/");

		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.025);
		hp--;
		speed = 0.025;
		auto effect = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("SlashEffect");
		effect.lock()->transform->SetWorldPosition((gameObject.lock()->transform->GetWorldPosition()));
		//velocity = Vector3();
		//moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition()-arg_other.lock()->transform->GetWorldPosition() )).GetNormalize()*2.0f;
		if (hp <= 0) {
			controller->DestroyBoss(score);
			gameObject.lock()->SetIsRemove(true);
			gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("Explosion", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
			gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->GetBehavior< CameraMan>()->ShakeHorizontal(0.3f);
			gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->GetBehavior< CameraMan>()->ShakeVartical(0.3f);
		}
		else {

			gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->GetBehavior< CameraMan>()->ShakeHorizontal(0.2f);

		}
	}
}

void ButiEngine::BossEnemy::OnShowUI()
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
}

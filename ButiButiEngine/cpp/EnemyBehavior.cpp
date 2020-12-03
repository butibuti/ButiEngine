#include "stdafx.h"
#include "..\include\EnemyBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include"include/GameController.h"
#include"Header/GameObjects/DefaultGameComponent/CameraMan.h"

void ButiEngine::EnemyBehavior::Start()
{
	gameObject.lock()->SetGameObjectTag(gameObject.lock()-> GetApplication().lock()->GetGameObjectTagManager()->GetObjectTag("Enemy"));


	controller = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("GameController").lock()->GetGameComponent<GameController>();

	stagemax = controller->GetStageMax();
	stagemin = controller->GetStageMin();
}

void ButiEngine::EnemyBehavior::OnUpdate()
{
	moveForce*= 0.7f;
	gameObject.lock()->transform->Translate(velocity*speed * GameDevice::WorldSpeed +moveForce * GameDevice::WorldSpeed);


	auto pos = gameObject.lock()->transform->GetWorldPosition();
	if (pos.x < stagemin.x) {
		pos.x = stagemin.x;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}
	else if (pos.x > stagemax.x) {
		pos.x = stagemax.x;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}
	if (pos.z < stagemin.z-10) {
		pos.z = stagemin.z;
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::EnemyBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag()==gameObject.lock()->GetApplication().lock()->GetGameObjectTagManager()->GetObjectTag("Bomb")) {

		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Slash.wav", "Sound/");

		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.025);
		hp--;
		speed = 0.025;
		auto effect = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("SlashEffect");
		effect.lock()->transform->SetWorldPosition((gameObject.lock()->transform->GetWorldPosition()));//velocity = Vector3();
		//moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition()-arg_other.lock()->transform->GetWorldPosition() )).GetNormalize()*2.0f;
		if (hp <= 0) {
			controller->AddScore(score);
			gameObject.lock()->SetIsRemove(true);
			gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("Explosion",ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
			gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->GetBehavior< CameraMan>()->ShakeHorizontal(0.3f);
		}
		else {

			gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->GetBehavior< CameraMan>()->ShakeHorizontal(0.2f);

		}
	}
}

void ButiEngine::EnemyBehavior::OnShowUI()
{
	GUI::BulletText("HP");
	int refHp = hp;
	GUI::DragInt("##hp",&refHp);
	hp = refHp;
	GUI::BulletText("Velocity");

	if (GUI::DragFloat3("##velocity", &velocity.x, 0.01f, -10, 10, "%.3f")) {
		velocity.Normalize();
	}
	GUI::BulletText("Speed");

	GUI::DragFloat("##speed", &speed, 0.01f,-1, 1);

	GUI::BulletText("Score");
	GUI::DragInt("##score", &score, 1, 0, 100);
}

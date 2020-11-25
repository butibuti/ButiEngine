#include "stdafx.h"
#include "Bullet.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::Bullet::Start()
{

	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::GetObjectTag("Enemy"));
}

void ButiEngine::Bullet::OnUpdate()
{
	gameObject.lock()->transform->Translate(velocity * speed);
	t--;
	if (t <= 0) {
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Bullet::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag() != GameObjectTagManager::GetObjectTag("Enemy")) {
		gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("BulletExplosion", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));

		gameObject.lock()->SetIsRemove(true);

		if (arg_other.lock()->GetGameObjectTag() == GameObjectTagManager::GetObjectTag("Bomb")) {

			auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Slash.wav", "Sound/");

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.025);
		}
	}
}

void ButiEngine::Bullet::OnShowUI()
{
}

void ButiEngine::Bullet::SetVelocity(const Vector3& arg_velocity)
{
	velocity = arg_velocity;
}

void ButiEngine::Bullet::SetSpeed(const float arg_speed)
{
	speed = arg_speed;
}

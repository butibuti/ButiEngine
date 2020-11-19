#include "stdafx.h"
#include "Bullet.h"

void ButiEngine::Bullet::Start()
{

	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::GetObjectTag("Enemy"));
}

void ButiEngine::Bullet::OnUpdate()
{
	gameObject.lock()->transform->Translate(velocity * speed);
}

void ButiEngine::Bullet::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag() != GameObjectTagManager::GetObjectTag("Enemy")) {
		
		gameObject.lock()->SetIsRemove(true);
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

#include "stdafx.h"
#include "BulletBehavior.h"
#include"Header/GameParts/ResourceContainer.h"

ButiEngine::StraightBulletBehavior::StraightBulletBehavior(const Vector3& arg_velocity, const float arg_speed, const std::vector<GameObjectTag>& arg_vec_hitTags)
{
	bulletVelocity = arg_velocity.GetNormalize();
	speed = arg_speed;
	vec_hitTags = arg_vec_hitTags;
}

void ButiEngine::StraightBulletBehavior::OnUpdate()
{
}

void ButiEngine::StraightBulletBehavior::OnSet()
{
	bulletSoundTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("bullet_hit.wav", "Sound/");
}

void ButiEngine::StraightBulletBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	for (auto itr = vec_hitTags.begin(); itr != vec_hitTags.end(); itr++) {
		if (arg_other.lock()->GetGameObjectTag() == *itr) {
			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(bulletSoundTag);
			gameObject.lock()->SetIsRemove(true);
			break;
		}
	}
}
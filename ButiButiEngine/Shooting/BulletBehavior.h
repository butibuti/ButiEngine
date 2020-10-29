#pragma once
#include"Header/BehaviorHeader.h"
namespace ButiEngine {
	class StraightBulletBehavior :public Behavior
	{

	public:
		StraightBulletBehavior(const Vector3& arg_velocity, const float arg_speed, const std::vector<GameObjectTag>& arg_vec_hitTags);
		std::string GetBehaviorName()override {
			return "BulletBehavior";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
	private:
		SoundTag bulletSoundTag;
		float speed;
		Vector3 bulletVelocity;
		std::vector<GameObjectTag> vec_hitTags;
	};

}
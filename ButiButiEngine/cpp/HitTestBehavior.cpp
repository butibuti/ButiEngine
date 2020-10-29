#include "stdafx.h"
#include "include/HitTestBehavior.h"

void ButiEngine::HitTestBehavior::OnUpdate()
{

	if (GameDevice::input.CheckKey(Keys::H)) {
		gameObject.lock()->transform->RollLocalRotationX_Degrees(1);
	}
	if (GameDevice::input.CheckKey(Keys::J)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(1);
	}
	if (GameDevice::input.CheckKey(Keys::K)) {
		gameObject.lock()->transform->RollLocalRotationZ_Degrees(1);
	}
}

void ButiEngine::HitTestBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
	//std::cout << "OnCollision:"<<gameObject.lock()->GetObjectName()<< std::endl;
}

void ButiEngine::HitTestBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	std::cout << "OnCollisionEnter:" << gameObject.lock()->GetObjectName() << std::endl;
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::HitTestBehavior::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
	std::cout << "OnCollisionEnd:" << gameObject.lock()->GetObjectName() << std::endl;
}

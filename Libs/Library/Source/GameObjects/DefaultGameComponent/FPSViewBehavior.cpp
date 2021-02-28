#include "stdafx.h"
#include"Header/GameParts/SoundManager.h"

void ButiEngine::FPSViewBehavior::Start()
{
}

void ButiEngine::FPSViewBehavior::OnUpdate()
{
	if (isCenter) {
		Vector2 mouseMove = GameDevice::GetInput()->GetMouseMove();
		GameDevice::GetInput()->SetMouseCursor(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetWindow()->GetWindowCenterPosition());
		gameObject.lock()->transform->RollLocalRotationX_Degrees(-mouseMove.y / 3.0f);
		gameObject.lock()->transform->RollWorldRotationY_Degrees(-mouseMove.x / 3.0f);
	}
	Vector3 moveForce;
	if (GameDevice::GetInput()->CheckKey(Keys::W)) {
		moveForce += gameObject.lock()->transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::S)) {
		moveForce -= gameObject.lock()->transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::A)) {
		moveForce -= gameObject.lock()->transform->GetRight();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		moveForce += gameObject.lock()->transform->GetRight();

	}
	if (GameDevice::GetInput()->TriggerKey(Keys::F)) {

		isCenter = !isCenter;
	}
	moveForce.y = 0;
	moveForce.Normalize();
	gameObject.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + moveForce * 0.05f);



}

void ButiEngine::FPSViewBehavior::OnSet()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::FPSViewBehavior::Clone()
{
	return ObjectFactory::Create<FPSViewBehavior>();
}

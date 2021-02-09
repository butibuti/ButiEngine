#include"stdafx_u.h"
#include "TestBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/Common/CollisionPrimitive.h"

void ButiEngine::TestBehavior::OnUpdate()
{
	/*static bool isForcus = false;

	if (isForcus) {

		Vector2 mouseMove = GameDevice::GetInput()->GetMouseMove();
		GameDevice::GetInput()->SetMouseCursor(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetWindow()->GetWindowCenterPosition());
		shp_camera->shp_transform->RollLocalRotationX_Degrees(-mouseMove.y / 3.0f);
		shp_camera->shp_transform->RollWorldRotationY_Degrees(-mouseMove.x / 3.0f);
	}*/
	Vector3 moveForce;
	if (GameDevice::GetInput()->CheckKey(Keys::W)) {
		moveForce += gameObject.lock()->transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::S)) {
		moveForce -= gameObject.lock()->transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::A)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(1);

	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-1);

	}
	if (GameDevice::GetInput()->CheckKey(Keys::LeftShift)) {
		moveForce += gameObject.lock()->transform->GetUp();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::Space)) {
		moveForce -= gameObject.lock()->transform->GetUp();

	}/*
	if (GameDevice::GetInput()->TriggerKey(Keys::Space)) {
		isForcus = !isForcus;

	}*/
	moveForce.Normalize();
	gameObject.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + moveForce * 0.1f);
	
	if (shp_timer->Update()) {
		shp_timer->Stop();
		//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ReloadScene();
	}

}

void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
	shp_timer = ObjectFactory::Create<RelativeTimer>(5);
	shp_timer->Start();
}

void ButiEngine::TestBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::TestBehavior::Clone()
{
	return ObjectFactory::Create<TestBehavior>();
}
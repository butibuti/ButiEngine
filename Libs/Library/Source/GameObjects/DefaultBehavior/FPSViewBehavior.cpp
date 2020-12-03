#include "stdafx.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/SoundManager.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\FPSViewBehavior.h"

BUTI_REGIST_BEHAVIOR(ButiEngine::FPSViewBehavior);
void ButiEngine::FPSViewBehavior::Start()
{
	auto headTransform = ObjectFactory::Create<Transform>(Vector3(0, 2.0f, 0.0f));
	headTransform->SetBaseTransform(gameObject.lock()->transform,true);
	head = GetManager().lock()->AddObject(headTransform, "head");
	gameObject.lock()->AddChildGameObject(head);


	auto camera = GetCamera("main");
	camera.lock()->shp_transform = head.lock()->transform;
}

void ButiEngine::FPSViewBehavior::OnUpdate()
{
	if (isCenter) {
		Vector2 mouseMove = GameDevice::GetInput()->GetMouseMove();
		GameDevice::GetInput()->SetMouseCursor(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetWindow()->GetWindowCenterPosition());
		head.lock()->transform->RollLocalRotationX_Degrees(-mouseMove.y / 3.0f);
		head.lock()->transform->RollWorldRotationY_Degrees(-mouseMove.x / 3.0f);
	}
	Vector3 moveForce;
	if (GameDevice::GetInput()->CheckKey(Keys::W)) {
		moveForce += head.lock()->transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::S)) {
		moveForce -= head.lock()->transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::A)) {
		moveForce -= head.lock()->transform->GetRight();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		moveForce += head.lock()->transform->GetRight();

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

std::shared_ptr<ButiEngine::Behavior> ButiEngine::FPSViewBehavior::Clone()
{
	return ObjectFactory::Create<FPSViewBehavior>();
}

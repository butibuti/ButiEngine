#include"stdafx_u.h"
#include "TestBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/Common/CollisionPrimitive.h"

void ButiEngine::TestBehavior::OnUpdate()
{

	Vector2 mouseMove = GameDevice::GetInput()->GetMouseMove();
	GameDevice::GetInput()->SetMouseCursor(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetWindow()->GetWindowCenterPosition());
	shp_camera->shp_transform->RollLocalRotationX_Degrees(-mouseMove.y / 3.0f);
	shp_camera->shp_transform->RollWorldRotationY_Degrees(-mouseMove.x / 3.0f);
	Vector3 moveForce;
	if (GameDevice::GetInput()->CheckKey(Keys::W)) {
		moveForce += shp_camera->shp_transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::S)) {
		moveForce -= shp_camera->shp_transform->GetFront();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::A)) {
		moveForce -=shp_camera->shp_transform->GetRight();

	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		moveForce += shp_camera->shp_transform->GetRight();

	}
	moveForce.y = 0;
	moveForce.Normalize();
	gameObject.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + moveForce * 0.05f);

}

void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
    //auto gameObject = GetManager().lock()->AddObjectFromCereal("test", ObjectFactory::Create<Transform>());
    //shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(1,1,1), gameObject.lock()->transform);

    shp_camera = GetCamera("main").lock();
	shp_camera->shp_transform->SetWorldPosition(Vector3());
	shp_camera->shp_transform->SetBaseTransform(gameObject.lock()->transform, true);
}

void ButiEngine::TestBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::TestBehavior::Clone()
{
    return ObjectFactory::Create<TestBehavior>();
}

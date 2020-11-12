#include "stdafx.h"
#include "..\include\PlayerBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::PlayerBehavior::Start()
{
	initPos = gameObject.lock()->transform->GetLocalPosition();
	initRotate = gameObject.lock()->transform->GetLocalRotation();
	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::CreateGameObjectTag("Player"));
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->ActiveCollision(true);
	auto prim= ObjectFactory::Create<Collision::CollisionPrimitive_Box_OBB>(Vector3(2, 3, 5), gameObject.lock()->transform);
	gameObject.lock()->AddGameComponent< Collision::ColliderComponent>(prim);
	
	auto bomb = gameObject.lock()->GetGameObjectManager().lock()->AddObject(ObjectFactory::Create<Transform>(Vector3(0,0,5),Vector3(),Vector3(2,2,2)), "bomb");
	bomb.lock()->SetGameObjectTag(GameObjectTagManager::CreateGameObjectTag("Bomb"));
	bomb.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	bomb.lock()->AddGameComponent<MeshDrawComponent>(gameObject.lock()->GetResourceContainer()->GetMeshTag("Sphere_UV_Normal"), gameObject.lock()->GetResourceContainer()->GetShaderTag("DefaultMesh"), gameObject.lock()->GetResourceContainer()->GetMaterialTag("childMaterial.bma","Material/"));
	auto spherePrim = ObjectFactory::Create<Collision::CollisionPrimitive_Sphere>(1.0f,bomb.lock()->transform);
	bomb.lock()->AddGameComponent< Collision::ColliderComponent>(spherePrim);

}

void ButiEngine::PlayerBehavior::OnUpdate()
{
	if (GameDevice::input.CheckKey(Keys::A)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-controllPase);
	}
	else if (GameDevice::input.CheckKey(Keys::D)) {

		gameObject.lock()->transform->RollLocalRotationY_Degrees(controllPase);
	}

	Vector3 velocity = gameObject.lock()->transform->GetFront();
	
	velocity *= speed;
	inertia *= inertiaMinorPase;
	velocity += inertia;

	gameObject.lock()->transform->Translate(velocity);

	inertia += velocity * 0.1f;

	if (gameObject.lock()->transform->GetLocalPosition().z > 50) {
		gameObject.lock()->transform->SetLocalPosition(initPos);
		gameObject.lock()->transform->SetLocalRotation(initRotate);
	}
}

void ButiEngine::PlayerBehavior::OnShowUI()
{
	ImGui::BulletText("Speed");
	if (ImGui::SliderFloat("##speed", &speed, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("Air resistance");
	if (ImGui::SliderFloat("##inertiaMinorPase", &inertiaMinorPase, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("ControllPase");
	if (ImGui::SliderFloat("##controllPase", &controllPase, 0.0f, 10.0f, "%.3f", 1.0f)) {
	}
}

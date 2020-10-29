#include "stdafx.h"
#include "HavokBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"BulletBehavior.h"
#include"SucideComponent.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::HavokBehavior::OnUpdate()
{
	if (GameDevice::input.GetPadButton(PadButtons::XBOX_BUTTON_RIGHT)) {
		Trigger();
	}
	else {
		shp_coolTimer->Update_UnExecute();
	}
}

void ButiEngine::HavokBehavior::OnSet()
{
	shp_coolTimer = ObjectFactory::Create<TimeBomb<HavokBehavior>>(10.0f, GetThis<HavokBehavior>());
	shp_coolTimer->bombFunc = &HavokBehavior::BulletShot;

	shp_coolTimer->Start();
}

void ButiEngine::HavokBehavior::Trigger()
{
	shp_coolTimer->Update();
}

void ButiEngine::HavokBehavior::BulletShot()
{
	auto bullet = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>((gameObject.lock()->transform->GetWorldPosition()), Vector3(), Vector3(0.5f, 0.5f, 0.5f)));

	bullet.lock()->AddBehavior<StraightBulletBehavior>(gameObject.lock()->transform->GetFront(), 0.5f, std::vector<GameObjectTag> { GameObjectTagManager::CreateGameObjectTag("Enemy") });

	bullet.lock()->AddGameComponent<MeshDrawComponent>(
		gameObject.lock()->GetResourceContainer()->GetMeshTag("TestCapsule"), gameObject.lock()->GetResourceContainer()->GetShaderTag("DefaultMesh"), gameObject.lock()-> GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/")
		);
	auto sphereCollision2 = ObjectFactory::Create<Collision::CollisionPrimitive_Sphere>(0.4f, bullet.lock()->transform);


	bullet.lock()->AddGameComponent<Collision::ColliderComponent>(sphereCollision2, 0);

	bullet.lock()->AddGameComponent<SucideComponent>(60.0f);
}

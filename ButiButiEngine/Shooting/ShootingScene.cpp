#include "stdafx.h"
#include "ShootingScene.h"
#include"MaguroBehavior.h"
#include"include/HitTestBehavior.h"
#include"SucideComponent.h"
#include"include/FPSViewBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/PararellDrawComponent.h"
#include"Header/GameObjects/DefaultBehavior/SampleBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"

ButiEngine::ShootingScene::ShootingScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation sceneInformation)
	:Scene(arg_wkp_sceneManager,sceneInformation)
{
}

ButiEngine::ShootingScene::~ShootingScene()
{
}

void ButiEngine::ShootingScene::OnUpdate()
{
}

void ButiEngine::ShootingScene::OnInitialize()
{


	//auto maguro = shp_gameObjectManager->AddObject(
	//	ObjectFactory::Create<Transform>(Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f)), "maguro"
	//);


	// auto maguroDraw= maguro.lock()->AddGameComponent<ModelDrawComponent>(
	//		GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), GetResourceContainer()->GetShaderTag("QuadModel"), BlendMode::AlphaBlend
	//		); 


	//maguro.lock()->AddBehavior<MaguroBehavior>();

	//auto obbCollision2 = ObjectFactory::Create<Collision::CollisionPrimitive_Box_OBB>(Vector3(1.5, 1.0, 3), maguro.lock()->transform);

	//maguro.lock()->AddGameComponent<Collision::ColliderComponent>(obbCollision2, 0);

	//auto spliteShower= shp_gameObjectManager->AddObject(
	//	ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(500.0f, 500.0f, 0.0f)), "backGround"
	//);

	//auto backDraw = spliteShower.lock()->AddGameComponent<MeshDrawComponent>(
	//	GetResourceContainer()->GetMeshTag("Floor"), GetResourceContainer()->GetShaderTag("DefaultMesh"), GetResourceContainer()->GetMaterialTag("haikei"), BlendMode::AlphaBlend, 1
	//	);
	//auto uiDraw = spliteShower.lock()->AddGameComponent<MeshDrawComponent>(
	//	GetResourceContainer()->GetMeshTag("Floor"),GetResourceContainer()->GetShaderTag("DefaultMesh"), GetResourceContainer()->GetMaterialTag("sorena"), BlendMode::AlphaBlend, 2,ObjectFactory::Create<Transform>(Vector3(-220,190,0),Vector3(),Vector3(200,200,0))
	//	);


	//auto floor = shp_gameObjectManager->AddObject(ObjectFactory::Create<Transform>(Vector3(0, 0, 0), Vector3(90, 0, 0), Vector3(500.0f, 500.0f, 500.0f)), "floor");

	//floor.lock()->AddGameComponent<MeshDrawComponent>(
	//	GetResourceContainer()->GetMeshTag("Floor"),GetResourceContainer()->GetShaderTag("Glid"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), BlendMode::AlphaBlend
	//	);


	//int range = 3;
	//for (int i = range; i < range * 2; i++)
	//{
	//	for (int j = range; j < range * 2; j++) {

	//		auto collisionObj2 = shp_gameObjectManager->AddObject(
	//			ObjectFactory::Create<Transform>(Vector3(-1.875 * i, 2, 1.875 * j), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f)), "obj:" + std::to_string(i) + "," + std::to_string(j)
	//		);
	//		collisionObj2.lock()->AddGameComponent<MeshDrawComponent>(
	//			GetResourceContainer()->GetMeshTag("TestCube"), GetResourceContainer()->GetShaderTag("DefaultMesh"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), BlendMode::AlphaBlend
	//			);
	//		auto obbCollision2 = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(1.0f, 1.0f, 1.0f), collisionObj2.lock()->transform);


	//		collisionObj2.lock()->AddGameComponent<Collision::ColliderComponent>(obbCollision2, 0);


	//		collisionObj2.lock()->AddBehavior<HitTestBehavior>();
	//		collisionObj2.lock()->SetGameObjectTag(GameObjectTagManager::CreateGameObjectTag("Enemy"));

	//	}
	//}
}

void ButiEngine::ShootingScene::PreInitialize()
{
}

void ButiEngine::ShootingScene::OnSceneEnd()
{
}
#pragma once
#include "stdafx.h"
#include "BeforeComponentsLoader.h"

#include"Header/GameParts/ResourceContainer.h"

#include"Header/GameObjects/DefaultBehavior/FPSViewBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimanotorComponent.h"
#include "Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include "Header/GameObjects/DefaultGameComponent/CameraMan.h"
#include "include/PlayerBehavior.h"
#include "include/EnemyBehavior.h"
#include "include/GameController.h"

void ButiEngine::BeforeComponentsLoader::BeforeBehaviorLoad(std::vector<std::shared_ptr<Behavior>>& vec_shp_addBehavior)
{



	//vec_shp_addBehavior.push_back(ObjectFactory::Create<SampleBehavior>());
	vec_shp_addBehavior.push_back(ObjectFactory::Create<FPSViewBehavior>());

	vec_shp_addBehavior.push_back(ObjectFactory::Create<PlayerBehavior>());
	vec_shp_addBehavior.push_back(ObjectFactory::Create<EnemyBehavior>());
	vec_shp_addBehavior.push_back(ObjectFactory::Create<CameraMan>());

}

void ButiEngine::BeforeComponentsLoader::BeforeComponentsLoad(std::vector<std::shared_ptr<GameComponent>>& vec_shp_addComponents, std::shared_ptr<Application>& arg_app)
{

	vec_shp_addComponents.push_back(ObjectFactory::Create<ModelDrawComponent>(
		arg_app->GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), arg_app->GetResourceContainer()->GetShaderTag("QuadModel"), nullptr
		));

	auto drawInfo = ObjectFactory::Create<DrawInformation>();
	vec_shp_addComponents.push_back(ObjectFactory::Create<MeshDrawComponent>(
		arg_app->GetResourceContainer()->GetMeshTag("Cube_UV_Normal"), arg_app->GetResourceContainer()->GetShaderTag("DefaultMesh"), arg_app->GetResourceContainer()->GetMaterialTag("childMaterial.bma", "Material/"), drawInfo
		));
	//vec_shp_addComponents.push_back(ObjectFactory::Create<ChaseComponent>(nullptr));
	vec_shp_addComponents.push_back(ObjectFactory::Create<LookAtComponent>(nullptr));
	//vec_shp_addComponents.push_back(ObjectFactory::Create<SucideComponent>(60.0f));
	vec_shp_addComponents.push_back(ObjectFactory::Create<GameController>());
}

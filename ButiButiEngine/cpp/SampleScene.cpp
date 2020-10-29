#pragma once
#include"stdafx.h"
#include"../include/SampleScene.h"
#include"include/FPSViewBehavior.h"
#include"include/HitTestBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/PararellDrawComponent.h"
#include"Header/GameObjects/DefaultBehavior/SampleBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimanotorComponent.h"
#include"Header/Resources/ModelAnimation.h"

ButiEngine::SampleScene::SampleScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation sceneInformation) 
	:Scene(arg_wkp_sceneManager, sceneInformation)
{

}


ButiEngine::SampleScene::~SampleScene()
{
}

void ButiEngine::SampleScene::OnUpdate()
{
	if (GameDevice::input.CheckKey(Keys::Y)) {
		shp_testGSVariable.lock()->Get().pushPower.w += 0.01f;
	}
	else  if (GameDevice::input.CheckKey(Keys::U)) {
		shp_testGSVariable.lock()->Get().pushPower.w -= 0.01f;
		if (shp_testGSVariable.lock()->Get().pushPower.w < 0) {
			shp_testGSVariable.lock()->Get().pushPower.w = 0;
		}
	}
	if (GameDevice::input.CheckKey(Keys::H)) {
		shp_testGSVariable.lock()->Get().bottom += 0.02f;
	}
	else  if (GameDevice::input.CheckKey(Keys::J)) {
		shp_testGSVariable.lock()->Get().bottom -= 0.02f;
	}
}

void ButiEngine::SampleScene::OnInitialize()
{



	/*auto maguro = shp_gameObjectManager->AddObject(
		ObjectFactory::Create<Transform>(Vector3(5, 2.0f, 0), Vector3(0, 0, 0), Vector3(1.0f,1.0f, 1.0f)), "maguro"
	);
	auto maguroModelComponent = maguro.lock()->AddGameComponent<ModelDrawComponent>(
		Application::GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), Application::GetResourceContainer()->GetShaderTag("QuadModel"), BlendMode::AlphaBlend
		);


	*/


	auto hikari = shp_gameObjectManager->AddObject(
		ObjectFactory::Create<Transform>(Vector3(0.0f, 0.0f, 1.0), Vector3(0, 0, 0), Vector3(0.5f, 0.5f, 0.5f)), "hikari"
	);
	auto info = ObjectFactory::Create<DrawInformation>();

	auto testVar = ObjectFactory::Create<CBuffer_Dx12< TestGSVariable>>(4);
	shp_testGSVariable = testVar;
	info->vec_exCBuffer.push_back(shp_testGSVariable.lock());
	//info->drawSettings.topologyType = TopologyType::point;
	auto hikariModelComponent = hikari.lock()->AddGameComponent<ModelDrawComponent>(
		GetResourceContainer()->GetModelTag("hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/"), GetResourceContainer()->GetShaderTag("PMXModel_GS"),info
		);


	//auto maguro = shp_gameObjectManager->AddObject(
	//	ObjectFactory::Create<Transform>(Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f)), "maguro"
	//);


	//auto maguroDraw = maguro.lock()->AddGameComponent<ModelDrawComponent>(
	//	GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), GetResourceContainer()->GetShaderTag("QuadModel"),nullptr
	//	);

	
	//hikariModelComponent->GetModelData()->SerchBoneByName(L"左ひじ")->transform->RollLocalRotationX_Degrees(90);




	auto animator= hikari.lock()->AddGameComponent<SimpleBoneAnimatorComponent>(hikariModelComponent->GetModelData());

	animator->AddAnimation(GetResourceContainer()->GetMotionTag("bakutyuu.bmd", "Motion/"));
	animator->SetLoop(true);

	auto checker = shp_gameObjectManager->AddObject(ObjectFactory::Create<Transform>(), "boneView");


	auto testTransform = ObjectFactory::Create<Transform>(Vector3(),  Vector3(),Vector3(2, 2, 2));
	auto hidarihiji = hikariModelComponent->GetModelData()->SerchBoneByName(L"左ひじ")->transform;
	hidarihiji->RollLocalRotationX_Degrees(45);
	testTransform->SetBaseTransform(	hidarihiji,true);
	checker.lock()->AddGameComponent<MeshDrawComponent>(
		GetResourceContainer()->GetMeshTag("TestCube"), GetResourceContainer()->GetShaderTag("DefaultMesh"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), nullptr, 0,testTransform
		); 
	
	auto floor = shp_gameObjectManager->AddObject(ObjectFactory::Create<Transform>(Vector3(0, -0.1, 0), Vector3(90, 0, 0), Vector3(50.0f, 50.0f, 50.0f)), "floor");

	floor.lock()->AddGameComponent<MeshDrawComponent>(
		GetResourceContainer()->GetMeshTag("Floor"), GetResourceContainer()->GetShaderTag("Glid"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), nullptr, 0
		);


	auto gsSphere = shp_gameObjectManager->AddObject(ObjectFactory::Create<Transform>( Vector3(5,1,0),Vector3(), Vector3(3,3,3)), "test");

	auto sphereInfo = ObjectFactory::Create<DrawInformation>();
	sphereInfo->drawSettings.topologyType = TopologyType::pointList;
	gsSphere.lock()->AddGameComponent<MeshDrawComponent>(
		GetResourceContainer()->GetMeshTag("TestCapsule"), GetResourceContainer()->GetShaderTag("DefaultMesh_test"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), sphereInfo, 0);

	auto player = shp_gameObjectManager->AddObject(
		ObjectFactory::Create<Transform>(Vector3(0.0f, 1.0f, 0.0f)), "player");

	player.lock()->AddBehavior<FPSViewBehavior>();



}

void ButiEngine::SampleScene::PreInitialize()
{
}

void ButiEngine::SampleScene::OnSceneEnd()
{
}
#pragma once
#include"stdafx.h"
#include"../include/SampleScene.h"
#include"Header/GameObjects/DefaultBehavior/FPSViewBehavior.h"
#include"include/HitTestBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
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

	ImGui::Begin("Bara Bara Slider");
	ImGui::SliderFloat("pushPower", &t, 0, 1.0f);
	ImGui::End();
	
	
	shp_testGSVariable.lock()->Get().pushPower.w =t*2;
	
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



	//auto maguro = shp_gameObjectManager->AddObject(
	//	ObjectFactory::Create<Transform>(Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f)), "maguro"
	//);


	//auto maguroDraw = maguro.lock()->AddGameComponent<ModelDrawComponent>(
	//	GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), GetResourceContainer()->GetShaderTag("QuadModel"),nullptr
	//	);

	
	//hikariModelComponent->GetModelData()->SerchBoneByName(L"¶‚Ð‚¶")->transform->RollLocalRotationX_Degrees(90);



	
	auto floor = shp_gameObjectManager->AddObjectFromCereal("floor.gameObject");


	
	auto player = shp_gameObjectManager->AddObjectFromCereal_Insert("player.gameObject");


}

void ButiEngine::SampleScene::PreInitialize()
{
}

void ButiEngine::SampleScene::OnSceneEnd()
{
}
#pragma once
#include"stdafx.h"

#include "Header/Scene/ComponentsLoader.h"

#include"Header/GameParts/ResourceContainer.h"

#include"include/GameController.h"
#include"Header/Device/ModelFileConverter.h"
#include "ScoreUI.h"
using namespace::ButiEngine;

#ifdef DEBUG

int main(){
std::cout << "debug" << std::endl;

#else

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//int main()
{
#endif

	auto app = ObjectFactory::Create<Application>();

	app->CreateInstances("ButiSampleApp", WindowPopType::max, 1260, 720);
	ButiRandom::Initialize();
	GameDevice::input.Initialize(app);

	
	app->InitLoadResources();
	//ModelFileConverter::FBXtoB3M("block.fbx", "block.b3m", "Model/AirBattle/");
	//ModelFileConverter::FBXtoB3M("enemy.fbx", "enemy.b3m", "Model/AirBattle/");

	std::shared_ptr<ComponentsLoader> componentsLoader = ObjectFactory::CreateFromCereal<ComponentsLoader>(GlobalSettings::GetResourceDirectory()+ "Application/componentLoader.loader");

	//componentsLoader->AddBehavior<Bullet>();
	//componentsLoader->AddBehavior<EnemyBehavior>();

	//componentsLoader->RemoveComponent("Booster");
	componentsLoader->AddGameComponent<ScoreUI>();

	componentsLoader->CreateNameList();

	app->GetSceneManager()->SetScene_Init("SampleScene", ObjectFactory::Create<EditScene>(app->GetSceneManager(), SceneInformation("SampleScene"), componentsLoader));
	app->GetSceneManager()->LoadScene("FailedScene", ObjectFactory::Create<Scene>(app->GetSceneManager(),SceneInformation("FailedScene")));
	app->GetGraphicDevice()->SetClearColor(Vector4(0.39,0.58,0.92,1));
	int returnCode = app->Run();

	OutputCereal(componentsLoader, GlobalSettings::GetResourceDirectory() + "Application/componentLoader.loader");

	componentsLoader->Release();
	app->Exit();


	return returnCode;
}
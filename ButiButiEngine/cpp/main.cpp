#pragma once
#include"stdafx.h"

#include "Header/Scene/ComponentsLoader.h"

#include"Header/GameParts/ResourceContainer.h"

#include"FailedSceneController.h"

#include"Header/Device/ModelFileConverter.h"
#include"Result.h"
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

	app->CreateInstances("JetSaber", WindowPopType::max, 1260, 720);
	ButiRandom::Initialize();
	GameDevice::input.Initialize(app);

	Result::CreateInstance();
	app->InitLoadResources();
	//ModelFileConverter::FBXtoB3M("enemy_2.fbx", "enemy_2.b3m", "Model/AirBattle/");
	//ModelFileConverter::FBXtoB3M("enemy_3.fbx", "enemy_3.b3m", "Model/AirBattle/");
	//ModelFileConverter::FBXtoB3M("Cloud.fbx", "Cloud.b3m", "Model/AirBattle/");

	std::shared_ptr<ComponentsLoader> componentsLoader = ObjectFactory::CreateFromCereal<ComponentsLoader>(GlobalSettings::GetResourceDirectory()+ "Application/componentLoader.loader");

	//componentsLoader->AddBehavior<Bullet>();
	//componentsLoader->AddBehavior<EnemyBehavior>();

	//componentsLoader->RemoveComponent("Booster");
	componentsLoader->AddGameComponent<ClearSceneController>();
	componentsLoader->CreateNameList();

	app->GetSceneManager()->SetScene_Init("TitleScene", ObjectFactory::Create<EditScene>(app->GetSceneManager(), SceneInformation("TitleScene"), componentsLoader));
	app->GetSceneManager()->LoadScene("LevelSelectScene", ObjectFactory::Create<Scene>(app->GetSceneManager(), SceneInformation("LevelSelectScene")));
	//p->GetSceneManager()->LoadScene("TitleScene", ObjectFactory::Create<Scene>(app->GetSceneManager(), SceneInformation("TitleScene")));
	//app->GetSceneManager()->LoadScene("FailedScene", ObjectFactory::Create<Scene>(app->GetSceneManager(), SceneInformation("FailedScene")));
	//app->GetSceneManager()->LoadScene("ClearScene", ObjectFactory::Create<Scene>(app->GetSceneManager(),SceneInformation("ClearScene")));
	app->GetGraphicDevice()->SetClearColor(Vector4(0.39,0.58,0.92,1));
	int returnCode = app->Run();

	OutputCereal(componentsLoader, GlobalSettings::GetResourceDirectory() + "Application/componentLoader.loader");

	componentsLoader->Release();
	app->Exit();
	Result::Output();

	return returnCode;
}
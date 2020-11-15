#pragma once
#include"stdafx.h"

#include "Header/Scene/ComponentsLoader.h"

#include"Header/GameParts/ResourceContainer.h"

//#include"include/EnemyBehavior.h"
//#include "Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
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

	GameDevice::input.Initialize(app);

	
	app->InitLoadResources();

	std::shared_ptr<ComponentsLoader> componentsLoader = ObjectFactory::CreateFromCereal<ComponentsLoader>(GlobalSettings::GetResourceDirectory()+ "Application/componentLoader.loader");

	componentsLoader->AddGameComponent<TransformAnimation>();

	

	componentsLoader->CreateNameList();

	app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<EditScene>(app->GetSceneManager(), SceneInformation("SampleScene"), componentsLoader));
	//app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<EditScene>(app->GetSceneManager(), SceneInformation("SampleScene"), componentsLoader));
	//app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<Scene>(app->GetSceneManager(),SceneInformation("EaseScene")));

	int returnCode = app->Run();

	OutputCereal(componentsLoader, GlobalSettings::GetResourceDirectory() + "Application/componentLoader.loader");

	componentsLoader->Release();
	app->Exit();


	return returnCode;
}
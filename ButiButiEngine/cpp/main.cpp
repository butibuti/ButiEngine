#pragma once
#include"stdafx.h"

#include "BeforeComponentsLoader.h"

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


	std::vector<std::shared_ptr<Behavior>>vec_shp_addBehavior;

	BeforeComponentsLoader::BeforeBehaviorLoad(vec_shp_addBehavior);

	std::vector<std::shared_ptr<GameComponent>>vec_shp_addComponents;

	BeforeComponentsLoader::BeforeComponentsLoad(vec_shp_addComponents, app);


	app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<EditScene>(app->GetSceneManager(), SceneInformation("SampleScene"), vec_shp_addBehavior, vec_shp_addComponents));
	//app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<Scene>(app->GetSceneManager(),SceneInformation("SampleScene")));

	int returnCode = app->Run();


	app->Exit();


	return returnCode;
}
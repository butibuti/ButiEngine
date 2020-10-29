#include"stdafx.h"
#include"Shooting/ShootingScene.h"
#include"../include/SampleScene.h"

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

	app->CreateInstances("ButiSampleApp", WindowPopType::max , 840, 480);

	GameDevice::input.Initialize(app);

	
	app->InitLoadResources();

	//Application::GetGame()->GetSceneManager().lock()->SetScene_Init("startScene", ObjectFactory::Create<StartScene>(SceneInformation()));
	//app->GetSceneManager()->SetScene_Init("playScene", ObjectFactory::Create<ShootingScene>(app->GetSceneManager(),SceneInformation()));


	app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<SampleScene>(app->GetSceneManager(),SceneInformation()));

	int returnCode = app->Run();


	app->Exit();


	return returnCode;
}
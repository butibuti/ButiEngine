#pragma once
#include"stdafx.h"

#include"Header/GameParts/Application.h"
using namespace::ButiEngine;

#ifdef DEBUG

int main(){
std::cout << "debug" << std::endl;

#else

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//int main()
{
#endif

	auto app = CreateDefaultApplicationInstance("JetSaber", WindowPopType::max, 1260, 720,true);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);


	app->InitLoadResources();

	app->GetSceneManager()->LoadScene_Init("CollisionScene");
	
	app->GetGraphicDevice()->SetClearColor(Vector4(0.39,0.58,0.92,1));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}
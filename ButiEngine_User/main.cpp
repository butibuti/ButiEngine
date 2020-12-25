#include"stdafx_u.h"
#include"Header/ApplicationCreater.h"
#include"Header/Scene/ComponentsLoader.h"
#include"TestBehavior.h"

#include"Header/Device/ModelFileConverter.h"
//#include"Header/GameObjects/DefaultGameComponent/ChaseComponent.h"
#include"Header/GameObjects/DefaultGameComponent/IKComponent.h"
#pragma comment(lib,"ButiEngine.lib")
using namespace::ButiEngine;

#ifdef DEBUG

int main() {
	std::cout << "debug" << std::endl;

#else

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//int main()
{
#endif
	auto app = CreateDefaultApplicationInstance("ButiSampleScene", WindowPopType::normal, 800,600, true);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);
	GameDevice::GetInput()->SetCursorHide(true);
	//ComponentsLoader::GetInstance()->AddGameComponent<IKComponent>();
	//ModelFileConverter::PMXtoB3M("ウルトラマンヒカリ._準ボーン.pmx", "hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/");
	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init("CRTScene");

	app->GetGraphicDevice()->SetClearColor(Vector4(0.39, 0.58, 0.92, 1));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}
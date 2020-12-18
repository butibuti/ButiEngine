#include"stdafx_u.h"
#include"Header/ApplicationCreater.h"
#pragma comment(lib,"ButiEngine.lib")
#include"Header/Scene/ComponentsLoader.h"
#include"TestBehavior.h"

#include"Header/Device/ModelFileConverter.h"
//#include"Header/GameObjects/DefaultGameComponent/ChaseComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
using namespace::ButiEngine;

#ifdef DEBUG

int main() {
	std::cout << "debug" << std::endl;

#else

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//int main()
{
#endif
	auto app = CreateDefaultApplicationInstance("ButiSampleScene", WindowPopType::normal, 1080,720, false);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);

	//ModelFileConverter::PMXtoB3M("ウルトラマンヒカリ._準ボーン.pmx", "hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/");
	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init_EditMode("DivisionScene");

	app->GetGraphicDevice()->SetClearColor(Vector4(0.39, 0.58, 0.92, 1));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}
#include"stdafx_u.h"
#include"Header/ApplicationCreater.h"
#include"Header/Scene/ComponentsLoader.h"
#include"TestBehavior.h"

#include"Header/Device/ModelFileConverter.h"
//#include"Header/GameObjects/DefaultGameComponent/ChaseComponent.h"
#include"Header/GameObjects/DefaultGameComponent/UIComponent.h"
#include"Header/GameObjects/DefaultGameComponent/UIAnimation.h"
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

	auto app = CreateDefaultApplicationInstance("DivisionScene", WindowPopType::max,1080,700, true);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);
	//ComponentsLoader::GetInstance()->AddGameComponent<IKComponent>();
	//ComponentsLoader::GetInstance()->AddGameComponent<UIComponent>();
	//ModelFileConverter::FBXtoB3M("maguro_leg_joint.fbx", "maguro_leg_joint.b3m", "Model/Maguro/");
	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init_EditMode("AABBSphereCollision");

	app->GetGraphicDevice()->SetClearColor(Vector4(0.39, 0.58, 0.92, 1));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}
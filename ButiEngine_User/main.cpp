#include"stdafx_u.h"
#include"Header/ApplicationCreater.h"
#include"Header/Scene/ComponentsLoader.h"
#include"TestBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/Device/ModelFileConverter.h"
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


	//auto app = CreateDefaultApplicationInstance("CollisionTest", WindowPopType::max, 1080, 700, false);
	auto app = CreateEditorApplicationInstance("DivisionScene", WindowPopType::max,1080,700, false);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);
	//ComponentsLoader::GetInstance()->AddGameComponent<MeshDrawComponent>();
	//ModelFileConverter::FBXtoB3M("maguro_leg_joint.fbx", "maguro_leg_joint.b3m", "Model/Maguro/");
	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init("Test");

	app->GetGraphicDevice()->SetClearColor(Vector4(0.5,0.5,0.8, 1));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}
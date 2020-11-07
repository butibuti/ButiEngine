#include"stdafx.h"
#include"Shooting/ShootingScene.h"
#include"../include/SampleScene.h"


#include"Header/GameParts/ResourceContainer.h"

#include"Header/GameObjects/DefaultBehavior/FPSViewBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ChaseComponent.h"
#include"Header/GameObjects/DefaultBehavior/SampleBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimanotorComponent.h"
#include "Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"

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



	std::vector<std::shared_ptr<GameComponent>>vec_shp_addComponents;

	vec_shp_addComponents.push_back(ObjectFactory::Create<ModelDrawComponent>(
		app->GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), app->GetResourceContainer()->GetShaderTag("QuadModel"), nullptr
		));

	//auto drawInfo = ObjectFactory::Create<DrawInformation>();
	vec_shp_addComponents.push_back(ObjectFactory::Create<MeshDrawComponent>(
		app->GetResourceContainer()->GetMeshTag("Plane_UV"), app->GetResourceContainer()->GetShaderTag("Glid"), app->GetResourceContainer()->GetMaterialTag("particleMaterial.bma", "Material/"), nullptr
		));
	//vec_shp_addComponents.push_back(ObjectFactory::Create<ChaseComponent>(player.lock()->transform));
	//vec_shp_addComponents.push_back(ObjectFactory::Create<LookAtComponent>(player.lock()->transform));
	vec_shp_addComponents.push_back(ObjectFactory::Create<SucideComponent>(60.0f));


	vec_shp_addBehavior.push_back(ObjectFactory::Create<SampleBehavior>());

	app->GetSceneManager()->SetScene_Init("sampleScene", ObjectFactory::Create<EditScene>(app->GetSceneManager(),SceneInformation(),vec_shp_addBehavior,vec_shp_addComponents));

	int returnCode = app->Run();


	app->Exit();


	return returnCode;
}
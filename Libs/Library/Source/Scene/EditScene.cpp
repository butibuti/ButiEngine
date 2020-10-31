#include"stdafx.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"


#include"include/FPSViewBehavior.h"
#include"include/HitTestBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/PararellDrawComponent.h"
#include"Header/GameObjects/DefaultBehavior/SampleBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SimpleBoneAnimanotorComponent.h"
#include"Header/Resources/ModelAnimation.h"
//#include "..\..\Header\Scene\EditScene.h"


void ButiEngine::EditScene::Update() {

	shp_gameObjectManager->RegistNewGameObject();
	if (isActive) {
		shp_gameObjectManager->Update();
		if (shp_collisionManager)
			shp_collisionManager->Update();

		OnUpdate();
		shp_soundManager->Update();
	}
	else {
		EditCamera();
	}

	UIUpdate();
}

void ButiEngine::EditScene::Set()
{
}

void ButiEngine::EditScene::OnSet()
{
}

void ButiEngine::EditScene::OnInitialize()
{
	auto hikari = shp_gameObjectManager->AddObject(
		ObjectFactory::Create<Transform>(Vector3(0.0f, 0.0f, 1.0), Vector3(0, 0, 0), Vector3(0.5f, 0.5f, 0.5f)), "hikari"
	);
	auto info = ObjectFactory::Create<DrawInformation>();

	auto testVar = ObjectFactory::Create<CBuffer_Dx12< TestGSVariable>>(4);
	shp_testGSVariable = testVar;
	info->vec_exCBuffer.push_back(shp_testGSVariable.lock());
	//info->drawSettings.topologyType = TopologyType::point;
	auto hikariModelComponent = hikari.lock()->AddGameComponent_Init<ModelDrawComponent>(
		GetResourceContainer()->GetModelTag("hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/"), GetResourceContainer()->GetShaderTag("PMXModel_GS"), info
		);


	//auto maguro = shp_gameObjectManager->AddObject(
	//	ObjectFactory::Create<Transform>(Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f)), "maguro"
	//);


	//auto maguroDraw = maguro.lock()->AddGameComponent<ModelDrawComponent>(
	//	GetResourceContainer()->GetModelTag("maguro.b3m", "Model/Maguro/"), GetResourceContainer()->GetShaderTag("QuadModel"),nullptr
	//	);


	//hikariModelComponent->GetModelData()->SerchBoneByName(L"左ひじ")->transform->RollLocalRotationX_Degrees(90);




	auto animator = hikari.lock()->AddGameComponent_Init<SimpleBoneAnimatorComponent>(hikariModelComponent->GetModelData());

	animator->AddAnimation(GetResourceContainer()->GetMotionTag("slash.bmd", "Motion/"));
	animator->SetLoop(true);


	auto floor = shp_gameObjectManager->AddObject(ObjectFactory::Create<Transform>(Vector3(0, -0.1, 0), Vector3(90, 0, 0), Vector3(50.0f, 50.0f, 50.0f)), "floor");

	floor.lock()->AddGameComponent_Init<MeshDrawComponent>(
		GetResourceContainer()->GetMeshTag("Floor"), GetResourceContainer()->GetShaderTag("Glid"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), nullptr, 0
		);


	auto gsSphere = shp_gameObjectManager->AddObject(ObjectFactory::Create<Transform>(Vector3(5, 2, 0), Vector3(0, 0, 0), Vector3(1, 1, 1)), "test");

	auto sphereInfo = ObjectFactory::Create<DrawInformation>();
	sphereInfo->drawSettings.billboardMode = BillBoardMode::y;
	gsSphere.lock()->AddGameComponent_Init<MeshDrawComponent>(
		GetResourceContainer()->GetMeshTag("Floor"), GetResourceContainer()->GetShaderTag("DefaultMesh"), GetResourceContainer()->GetMaterialTag("blueMaterial.bma", "Material/"), sphereInfo, 0);

	auto player = shp_gameObjectManager->AddObject(
		ObjectFactory::Create<Transform>(Vector3(0.0f, 1.0f, 0.0f)), "player");

	player.lock()->AddBehavior_Init<FPSViewBehavior>();
}

void ButiEngine::EditScene::OnUpdate()
{
	


	shp_testGSVariable.lock()->Get().pushPower.w = t * 2;

	if (GameDevice::input.CheckKey(Keys::H)) {
		shp_testGSVariable.lock()->Get().bottom += 0.02f;
	}
	else  if (GameDevice::input.CheckKey(Keys::J)) {
		shp_testGSVariable.lock()->Get().bottom -= 0.02f;
	}

}

void ButiEngine::EditScene::UIUpdate()
{
	ImGui::Begin("top");
	ImGui::Checkbox("isStart", &isActive);
	ImGui::End();


	/*ImGui::Begin("Bara Bara Slider");
	ImGui::SliderFloat("pushPower", &t, 0, 1.0f);
	ImGui::End();*/

	shp_gameObjectManager->ShowUI();

}

void ButiEngine::EditScene::EditCamera()
{
	if (GameDevice::input.GetMouseButton(MouseButtons::RightClick)) {
		Vector2 move = GameDevice::input.GetMouseMove();
		editCam->shp_transform->RollWorldRotationY_Degrees(-move.x);
		editCam->shp_transform->RollLocalRotationX_Degrees(-move.y);
	}
	if (GameDevice::input.GetMouseButton(MouseButtons::WheelButton)) {
		Vector2 move = GameDevice::input.GetMouseMove();

		Vector3 velocity = editCam->shp_transform->GetRight() * move.x * 0.01f + editCam->shp_transform->GetUp() * move.y*-1 * 0.01f;

		editCam->shp_transform->Translate (velocity);
	}
	if (GameDevice::input.GetMouseWheel()) {
		Vector3 velocity = editCam->shp_transform->GetFront() * 0.001f * GameDevice::input.GetMouseWheelMove();

		editCam->shp_transform->Translate(velocity);
	}
}

void ButiEngine::EditScene::Draw()
{

	shp_renderer->RenderingStart();


	shp_renderer->BefRendering();


	for (auto cameraItr = vec_cameras.begin(); cameraItr != vec_cameras.end(); cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}

		(*cameraItr)->Start();

		(*cameraItr)->Draw();
		shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		(*cameraItr)->Stop();
	}

	if (isActive) {

		mainCam->Start();

		mainCam->Draw();
		shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		mainCam->Stop();
	}
	else {

		editCam->Start();

		editCam->Draw();
		shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		editCam->Stop();
	}


	shp_renderer->RenderingEnd();
}

ButiEngine::EditScene::EditScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation argSceneInformation)
{
	shp_sceneManager = arg_wkp_sceneManager.lock();
}

void ButiEngine::EditScene::Release()
{
	shp_gameObjectManager = nullptr;
	shp_soundManager->Release();
	shp_renderer->Release();
	shp_sceneManager = nullptr;
}


void ButiEngine::EditScene::Initialize()
{
	shp_gameObjectManager = ObjectFactory::Create<GameObjectManager>(GetThis<IScene>());

	shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());

	shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());

	auto windowSize = GetWindow()->GetSize();/*
	shp_renderer->AddLayer();
	shp_renderer->AddLayer();
	auto prop2 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0,true,1);
	AddCamera(prop2, "backGround", true);*/

	auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
	prop.farClip = 100.0f;
	AddCamera(prop, "main", true);
	AddCamera(prop, "edit", true);

	GetCamera("edit").lock()->shp_transform->SetLocalPosition(Vector3(5, 5, -5));

	GetCamera("edit").lock()->shp_transform->SetLookAtRotation(Vector3(0, 0, 0));

	//auto prop3 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 2);
	//AddCamera(prop3, "backGround", true);
	OnInitialize();
}

void ButiEngine::EditScene::ActiveCollision(const UINT arg_layerCount)
{
	shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
}

void ButiEngine::EditScene::PreInitialize()
{
}

std::unique_ptr<ButiEngine::Window>& ButiEngine::EditScene::GetWindow()
{
	return shp_sceneManager->GetApplication().lock()->GetWindow();
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::GetCamera(const std::string& arg_camName)
{

	if (arg_camName == "main") {
		return mainCam;
	}
	if (arg_camName == "edit") {
		return editCam;
	}

	for (auto itr = vec_cameras.begin(); itr != vec_cameras.end(); itr++) {
		if ((*itr)->GetName() == arg_camName) {
			return (*itr);
		}
	}
	std::cout << "this camera name is wrong." << std::endl;
	return vec_cameras.at(0);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::GetCamera(const UINT arg_camNum)
{
	return vec_cameras.at(arg_camNum);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::AddCamera(const CameraProjProperty& arg_prop, const std::string arg_cameraName, const bool arg_initActive)
{

	if (arg_cameraName == "main") {

		auto out = CameraCreater::CreateCamera(arg_prop, arg_cameraName, arg_initActive, shp_renderer, shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
		mainCam = out;
		return out;
	}if (arg_cameraName == "edit") {

		auto out = CameraCreater::CreateCamera(arg_prop, arg_cameraName, arg_initActive, shp_renderer, shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
		editCam = out;
		return out;
	}

	auto out = CameraCreater::CreateCamera(arg_prop, arg_cameraName, arg_initActive, shp_renderer, shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
	vec_cameras.push_back(out);
	return out;
}

void ButiEngine::EditScene::RemoveCamera(const std::string& arg_camName)
{
	auto itr = vec_cameras.begin();
	while (itr != vec_cameras.end()) {
		if ((*itr)->GetName() == arg_camName) {
			itr = vec_cameras.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ButiEngine::EditScene::RemoveCamera(const UINT arg_camNum)
{
	auto itr = vec_cameras.begin();
	for (int i = 0; i < arg_camNum; i++) {
		itr++;
	}


	vec_cameras.erase(itr);
}

void ButiEngine::EditScene::SceneEnd()
{
	OnSceneEnd();
}

std::shared_ptr<ButiEngine::ResourceContainer> ButiEngine::EditScene::GetResourceContainer()
{
	return shp_sceneManager->GetApplication().lock()->GetResourceContainer();
}

std::shared_ptr<ButiEngine::IRenderer> ButiEngine::EditScene::GetRenderer()
{
	return shp_renderer;
}

std::shared_ptr<ButiEngine::ISoundManager> ButiEngine::EditScene::GetSoundManager()
{
	return shp_soundManager;
}

std::weak_ptr< ButiEngine::ISceneManager> ButiEngine::EditScene::GetSceneManager()
{
	return shp_sceneManager;
}

std::weak_ptr<ButiEngine::Collision::CollisionManager> ButiEngine::EditScene::GetCollisionManager()
{
	return shp_collisionManager;
}


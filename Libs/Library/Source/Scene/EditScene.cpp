#pragma once
#include"stdafx.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"


#include"Header/Resources/ModelAnimation.h"
#include"Header/Scene/ComponentsLoader.h"
#include"Header/Scene/EditScene.h"

#include"Header/GameParts/CollisionManager.h"


void ButiEngine::EditScene::Update() {

	shp_gameObjectManager->RegistNewGameObject();
	if (isActive) {

		shp_gameObjectManager->Update();
		if (shp_collisionManager)
			shp_collisionManager->Update();

		OnUpdate();
		shp_soundManager->Update();
	}

	UIUpdate();
}

void ButiEngine::EditScene::Set()
{
	shp_gameObjectManager->Start();
}

void ButiEngine::EditScene::OnSet()
{
}

void ButiEngine::EditScene::OnInitialize()
{
	
}

void ButiEngine::EditScene::OnUpdate()
{

}

void ButiEngine::EditScene::UIUpdate()
{
	
}

void ButiEngine::EditScene::Draw()
{

	shp_renderer->RenderingStart();


	shp_renderer->BefRendering();

	std::vector<std::shared_ptr<ICamera>> cams;
	for (auto cameraItr = vec_cameras.begin(); cameraItr != vec_cameras.end(); cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}

		cams.push_back(*cameraItr);
	}

	for (auto cameraItr = cams.begin(); cameraItr != cams.end(); cameraItr++) {

		(*cameraItr)->Start();

		(*cameraItr)->Draw();
		
		if (cameraItr + 1 == cams.end()) {
			shp_sceneManager->GetApplication().lock()->GetGraphicDevice()->SetDefaultRenderTarget();
			shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();
		}

		(*cameraItr)->Stop();
	}



	shp_renderer->RenderingEnd();

	for (auto cameraItr = cams.begin(); cameraItr != cams.end(); cameraItr++) {

		(*cameraItr)->End();
	}
}


ButiEngine::EditScene::EditScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, std::shared_ptr< SceneInformation> arg_SceneInformation)
	:sceneInformation ( arg_SceneInformation)
{
	
	shp_sceneManager = arg_wkp_sceneManager.lock();
}

void ButiEngine::EditScene::Release()
{
	
	shp_gameObjectManager->Release();
	shp_soundManager->Release();
	shp_renderer->ReleaseFogBuffer();
	shp_renderer->Release();
	(shp_collisionManager) = nullptr;
	shp_sceneManager = nullptr;
}


void ButiEngine::EditScene::Initialize()
{


	shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());
	{

		std::string fullRenderingInfoPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/rendering.renderingInfo";
		if (Util::IsFileExistence(fullRenderingInfoPath)) {
			shp_renderingInfo = std::make_shared<SceneRenderingInformation>(); 
			InputCereal (shp_renderingInfo, fullRenderingInfoPath);
		}
		else {

			shp_renderingInfo = std::make_shared<SceneRenderingInformation>();

			auto windowSize = GetWindow()->GetSize();
			auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
			prop.farClip = 200.0f;
			prop.cameraName = "main";
			shp_renderingInfo->vec_cameraProperty.push_back(prop);
			prop.cameraName = "edit";
			shp_renderingInfo->vec_cameraProperty.push_back(prop);


			shp_renderingInfo->vec_cameraTransform.push_back(ObjectFactory::Create<Transform>());
			auto editTransform = ObjectFactory::Create<Transform>(Vector3(5,5,-5));editTransform-> SetLookAtRotation(Vector3());
			shp_renderingInfo->vec_cameraTransform.push_back(editTransform);

			_mkdir((GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/").c_str());
		}
	}
	for (int i = 0; i < shp_renderingInfo->layerCount; i++) {

		shp_renderer->AddLayer();
	}
	shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());

	ActiveCollision(1);

	{

		std::string fullGameObjectManagerPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/objects.gameObjectManager";
		if (Util::IsFileExistence(fullGameObjectManagerPath)) {
			shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(fullGameObjectManagerPath);

			shp_gameObjectManager->SetScene(GetThis<IScene>());
			shp_gameObjectManager->Initialize_cereal();
		}
		else {
			auto defaultPath = GlobalSettings::GetResourceDirectory() + "Scene/" + "DefaultScene" + "/objects.gameObjectManager";
			shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(defaultPath);

			shp_gameObjectManager->SetScene(GetThis<IScene>());
			shp_gameObjectManager->Initialize_cereal();

			_mkdir((GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/").c_str());
		}
	} 

	bool isContainEditCam = false;
	for (int i = 0; i < shp_renderingInfo->vec_cameraTransform.size(); i++) {
		auto prop = shp_renderingInfo->vec_cameraProperty[i];
		AddCamera(prop, prop.cameraName, true).lock()->shp_transform=shp_renderingInfo->vec_cameraTransform[i];
		if (prop.cameraName == "edit") {
			isContainEditCam = true;
		}
	}
	if (!isContainEditCam) {
		auto windowSize = GetWindow()->GetSize();
		auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
		prop.cameraName = "edit";
		AddCamera(prop, "edit", true);
		shp_renderingInfo->vec_cameraProperty.push_back(prop);
		shp_renderingInfo->vec_cameraTransform.push_back(ObjectFactory::Create<Transform>());
	}
	GetCamera("edit").lock()->SetActive(true);
	GetCamera("main").lock()->SetActive(false);

	//auto prop3 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 2);
	//AddCamera(prop3, "backGround", true);
	OnInitialize();

}

void ButiEngine::EditScene::ActiveCollision(const UINT arg_layerCount)
{
	if (!shp_collisionManager) {
		std::string path = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/collision.collisionManager";
		if (Util::IsFileExistence(path)) {
			auto temp= ObjectFactory::Create<Collision::CollisionManager>();
			InputCereal(temp,path);
			temp->ReCreateLayers();
			shp_collisionManager = temp;
		}
		else
		{
			shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
		}
	}
}

void ButiEngine::EditScene::PreInitialize()
{
}

std::unique_ptr<ButiEngine::IWindow>& ButiEngine::EditScene::GetWindow()
{
	return shp_sceneManager->GetApplication().lock()->GetWindow();
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::GetCamera(const std::string& arg_camName)
{

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

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::AddCamera(CameraProjProperty& arg_prop, const std::string& arg_cameraName, const bool arg_initActive)
{
	bool isContain =true;
	int count = 0;
	while (isContain) {
		for (auto itr = vec_cameras.begin(); itr != vec_cameras.end(); itr++) {
			if ((*itr)->GetName() == arg_cameraName||(*itr)->GetName() == arg_cameraName + "_" + std::to_string(count)) {
				count++;
				isContain = true;
				break;
			}
		}
		isContain = false;
	}
	if (count > 0) {
		arg_prop.cameraName += "_""_" + std::to_string(count);
	}
	auto out = CameraCreater::CreateCamera(arg_prop, arg_prop.cameraName, arg_initActive, shp_renderer, shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
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

std::shared_ptr<ButiEngine::IResourceContainer> ButiEngine::EditScene::GetResourceContainer()
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

std::weak_ptr<ButiEngine::ICollisionManager> ButiEngine::EditScene::GetCollisionManager()
{
	return shp_collisionManager;
}

std::shared_ptr< ButiEngine::SceneInformation > ButiEngine::EditScene::GetSceneInformation()
{
	return sceneInformation;
}

std::shared_ptr<ButiEngine::SceneChangeInformation> ButiEngine::EditScene::GetSceneChangeInformation()
{
	return shp_changeInfo;
}

std::shared_ptr<ButiEngine::SceneRenderingInformation> ButiEngine::EditScene::GetSceneRenderingInformation()
{
	return shp_renderingInfo;
	
}


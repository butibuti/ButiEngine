#include"stdafx.h"
#include "..\..\Header\Scene\Scene.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"

void ButiEngine::Scene::Update() {
	shp_gameObjectManager->RegistNewGameObject();
	shp_gameObjectManager->Update();
	if(shp_collisionManager)
	shp_collisionManager->Update();
	OnUpdate();
	shp_soundManager->Update();
}

void ButiEngine::Scene::Set()
{
	shp_gameObjectManager->Start();
}

void ButiEngine::Scene::OnSet()
{
}

void ButiEngine::Scene::Draw()
{

	shp_renderer->RenderingStart();


	shp_renderer->BefRendering();



	for (auto cameraItr = vec_cameras.begin(); cameraItr != vec_cameras.end(); cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}

		(*cameraItr)->Start();

		(*cameraItr)->Draw();

		if (cameraItr + 1 == vec_cameras.end())
			shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		(*cameraItr)->Stop();
	}



	shp_renderer->RenderingEnd();
}

ButiEngine::Scene::Scene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, std::shared_ptr< SceneInformation> argSceneInformation):sceneInformation(argSceneInformation)
{
	shp_sceneManager = arg_wkp_sceneManager.lock();
}

void ButiEngine::Scene::Release()
{
	shp_gameObjectManager->Release();
	shp_soundManager->Release();
	shp_renderer->ReleaseFogBuffer();
	shp_renderer->Release();
	shp_sceneManager = nullptr;
}


void ButiEngine::Scene::Initialize()
{
	//shp_gameObjectManager = ObjectFactory::Create<GameObjectManager>(GetThis<IScene>());


	shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());
	shp_renderer->AddLayer();

	shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());

	ActiveCollision(1);
	auto windowSize = GetWindow()->GetSize();
	std::string fullGameObjectManagerPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/objects.gameObjectManager";
	if (Util::IsFileExistence(fullGameObjectManagerPath)) {
		shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(fullGameObjectManagerPath);

		shp_gameObjectManager->SetScene(GetThis<IScene>());
		shp_gameObjectManager->Initialize_cereal();
	}
	else {
		_mkdir(fullGameObjectManagerPath.c_str());
		shp_gameObjectManager = ObjectFactory::Create<GameObjectManager>(GetThis<IScene>());
	}

	auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
	prop.farClip = 150.0;
	AddCamera(prop, "main", true);
	auto prop2 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 1);
	AddCamera(prop2, "ui", true);

	//auto prop3 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 2);
	//AddCamera(prop3, "backGround", true);
	OnInitialize();

}

void ButiEngine::Scene::ActiveCollision(const UINT arg_layerCount)
{
	if (!shp_collisionManager) {
		shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
	}
	else if (arg_layerCount != shp_collisionManager->GetLayerCount())
		shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
}

void ButiEngine::Scene::PreInitialize()
{
}

std::unique_ptr<ButiEngine::IWindow>& ButiEngine::Scene::GetWindow()
{
	return shp_sceneManager->GetApplication().lock()->GetWindow();
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::Scene::GetCamera(const std::string& arg_camName)
{


	for (auto itr = vec_cameras.begin(); itr != vec_cameras.end(); itr++) {
		if ((*itr)->GetName() == arg_camName) {
			return (*itr);
		}
	}
	std::cout << "this camera name is wrong." << std::endl;
	return vec_cameras.at(0);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::Scene::GetCamera(const UINT arg_camNum)
{
	return vec_cameras.at(arg_camNum);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::Scene::AddCamera(const CameraProjProperty& arg_prop, const std::string arg_cameraName, const bool arg_initActive)
{


	auto out = CameraCreater::CreateCamera(arg_prop, arg_cameraName, arg_initActive,shp_renderer,shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
	vec_cameras.push_back(out);
	return out;
}

void ButiEngine::Scene::RemoveCamera(const std::string& arg_camName)
{
	auto itr = vec_cameras.begin();
	while ( itr != vec_cameras.end() ) {
		if ((*itr)->GetName() == arg_camName) {
			itr = vec_cameras.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ButiEngine::Scene::RemoveCamera(const UINT arg_camNum)
{
	auto itr = vec_cameras.begin();
	for (int i = 0; i < arg_camNum; i++) {
		itr++;
	}
	

	vec_cameras.erase(itr);
}

void ButiEngine::Scene::SceneEnd()
{
	OnSceneEnd();
}

std::shared_ptr<ButiEngine::IResourceContainer> ButiEngine::Scene::GetResourceContainer()
{
	return shp_sceneManager->GetApplication().lock()->GetResourceContainer();
}

std::shared_ptr<ButiEngine::IRenderer> ButiEngine::Scene::GetRenderer()
{
	return shp_renderer;
}

std::shared_ptr<ButiEngine::ISoundManager> ButiEngine::Scene::GetSoundManager()
{
	return shp_soundManager;
}

std::weak_ptr< ButiEngine::ISceneManager> ButiEngine::Scene::GetSceneManager()
{
	return shp_sceneManager;
}

std::weak_ptr<ButiEngine::Collision::CollisionManager> ButiEngine::Scene::GetCollisionManager()
{
	return shp_collisionManager;
}

std::shared_ptr< ButiEngine::SceneInformation >ButiEngine::Scene::GetSceneInformation()
{
	return sceneInformation;
}


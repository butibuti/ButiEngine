#include"stdafx.h"
#include "..\..\Header\Scene\Scene.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"

void ButiEngine::Scene::Update() {
	shp_gameObjectManager->Update();
	if(shp_collisionManager)
	shp_collisionManager->Update();
	OnUpdate();
	shp_soundManager->Update();
}

void ButiEngine::Scene::Set()
{
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

		(*cameraItr)->Stop();
	}
	shp_renderer->RenderingEnd();
}

ButiEngine::Scene::Scene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation argSceneInformation)
{
	shp_sceneManager = arg_wkp_sceneManager.lock();
}

void ButiEngine::Scene::Release()
{
	shp_gameObjectManager = nullptr;
	shp_soundManager->Release();
	shp_renderer->Release();
	shp_sceneManager = nullptr;
}


void ButiEngine::Scene::Initialize()
{
	shp_gameObjectManager = ObjectFactory::Create<GameObjectManager>(GetThis<IScene>());

	shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());
	
	shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());
	
	auto windowSize = GetWindow()->GetSize();
	shp_renderer->AddLayer();
	shp_renderer->AddLayer();
	auto prop2 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0,true,1);
	AddCamera(prop2, "backGround", true);

	auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
	prop.farClip = 100.0f;
	AddCamera(prop, "main", true);

	auto prop3 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 2);
	AddCamera(prop3, "backGround", true);
	OnInitialize();
}

void ButiEngine::Scene::ActiveCollision(const UINT arg_layerCount)
{
	shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
}

void ButiEngine::Scene::PreInitialize()
{
}

std::unique_ptr<ButiEngine::Window>& ButiEngine::Scene::GetWindow()
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

std::shared_ptr<ButiEngine::ResourceContainer> ButiEngine::Scene::GetResourceContainer()
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


#include"stdafx.h"
#include "..\..\Header\Scene\SceneManager.h"


ButiEngine::SceneManager::SceneManager(std::weak_ptr<Application> arg_wkp_app)
{
	sceneChangeTimer = ObjectFactory::Create<AbsoluteTimer>(10);
	wkp_app = arg_wkp_app;
}

void ButiEngine::SceneManager::Initialize()
{
}

void ButiEngine::SceneManager::PreInitialize()
{
}

void ButiEngine::SceneManager::LoadScene(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_argScene)
{
	if (!map_iscene.count(arg_sceneName)) {
		map_iscene.emplace(arg_sceneName, shp_argScene->GetThis<IScene>());
	}
}

void ButiEngine::SceneManager::RemoveScene(const std::string& arg_sceneName)
{
	if (map_iscene.count(arg_sceneName)) {
		map_iscene.at(arg_sceneName)->Release();
		map_iscene.erase(arg_sceneName);
	}
}

void ButiEngine::SceneManager::ChangeScene(std::string arg_sceneName, float sceneChangeDalay)
{
	if (!map_iscene.count(arg_sceneName)) {
		return;
	}
	newScene = map_iscene.at(arg_sceneName);
	sceneChangeTimer = ObjectFactory::Create<AbsoluteTimer>(sceneChangeDalay);
	sceneChangeTimer->Start();
}

void ButiEngine::SceneManager::SetScene_Init(const std::string & arg_sceneName, std::shared_ptr<IScene> arg_shp_Scene)
{
	if (!map_iscene.count(arg_sceneName)) {
		map_iscene.emplace(arg_sceneName, arg_shp_Scene->GetThis<IScene>());
	}
	currentScene = arg_shp_Scene;
}


void ButiEngine::SceneManager::RenewalScene()
{
	if(currentScene!=nullptr) 
		currentScene->SceneEnd();
	currentScene = newScene;
	newScene = nullptr;
	currentScene->Set();
}

std::weak_ptr<ButiEngine::Application> ButiEngine::SceneManager::GetApplication()
{
	return wkp_app;
}

void ButiEngine::SceneManager::Release()
{
	auto itr = map_iscene.begin();
	while (itr!=map_iscene.end())
	{
		itr->second->Release();
		itr = map_iscene.erase(itr);
	}
}



ButiEngine::SceneManager::~SceneManager()
{
	currentScene = nullptr;
	newScene = nullptr;
}



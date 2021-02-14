#pragma once
#include"stdafx.h"
#include "..\..\Header\Scene\SceneManager.h"

#include"..\..\Header\Scene/Scene.h"
#include <thread>

ButiEngine::SceneManager::SceneManager(std::weak_ptr<IApplication> arg_wkp_app)
{
	sceneChangeTimer = ObjectFactory::Create<AbsoluteTimer>(10);
	wkp_app = arg_wkp_app;
}

void SceneUpdate(std::shared_ptr<ButiEngine::IScene> currentScene) {
	currentScene->Update();
}

void SceneDraw(std::shared_ptr<ButiEngine::IScene> currentScene) {
	currentScene->Draw();
}
void ButiEngine::SceneManager::Update()
{
	if (isReload) {
		RemoveScene(currentScene->GetSceneInformation()->GetSceneName());
		LoadScene(currentScene->GetSceneInformation()->GetSceneName());
		ChangeScene(currentScene->GetSceneInformation()->GetSceneName());
		
		isReload = false;
	}

	if (sceneChangeTimer->Update())
	{
		RenewalScene();
		sceneChangeTimer->Stop();
	}

	GUI::SetState(GUI::GUIState::noActive);
	currentScene->UIUpdate();

	GUI::SetState(GUI::GUIState::enable);
	currentScene->BefDraw();

	auto updateThread = std::thread(SceneUpdate, currentScene);

	currentScene->Draw();


	wkp_app.lock()->GetGraphicDevice()->Update();

	wkp_app.lock()->GetGraphicDevice()->ResourceUploadRelease();
	updateThread.join();
}

void ButiEngine::SceneManager::Initialize()
{
}

void ButiEngine::SceneManager::PreInitialize()
{
}


void ButiEngine::SceneManager::RemoveScene(const std::string& arg_sceneName)
{
	if (map_iscene.count(arg_sceneName)) {
		map_iscene.at(arg_sceneName)->Release();
		map_iscene.erase(arg_sceneName);
	}
}

void ButiEngine::SceneManager::ChangeScene(const std::string& arg_sceneName, float sceneChangeDalay)
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
	//currentScene->Set();
}

void ButiEngine::SceneManager::SetScene(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_argScene)
{
	if (!map_iscene.count(arg_sceneName)) {
		map_iscene.emplace(arg_sceneName, shp_argScene->GetThis<IScene>());
	}
}
void ButiEngine::SceneManager::LoadScene(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_sceneInfo)
{
	if (!map_iscene.count(arg_sceneName)) {
		if (!shp_sceneInfo) {
			shp_sceneInfo = ObjectFactory::Create<SceneInformation>(arg_sceneName);
		}

		SetScene(arg_sceneName, ObjectFactory::Create<Scene>(GetThis<ISceneManager>(), shp_sceneInfo));
	}
}
void ButiEngine::SceneManager::LoadScene_Init(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_sceneInfo)
{
	if (!map_iscene.count(arg_sceneName)) {
		if (!shp_sceneInfo) {
			shp_sceneInfo = ObjectFactory::Create<SceneInformation>(arg_sceneName);
		}

		SetScene_Init(arg_sceneName, ObjectFactory::Create<Scene>(GetThis<ISceneManager>(), shp_sceneInfo));
		currentScene->Set();
		currentScene->Start();
	}
}

void ButiEngine::SceneManager::ReloadScene()
{
	isReload = true;
}

void ButiEngine::SceneManager::ReloadScene(const std::string& arg_sceneName)
{
	if (!map_iscene.count(arg_sceneName)) {
		return;
	}
	if (map_iscene[arg_sceneName] == currentScene) {
		ReloadScene();
		return;
	}
	


	RemoveScene(map_iscene[arg_sceneName]->GetSceneInformation()->GetSceneName());
	LoadScene(map_iscene[arg_sceneName]->GetSceneInformation()->GetSceneName());
	ChangeScene(map_iscene[arg_sceneName]->GetSceneInformation()->GetSceneName());
	
}


void ButiEngine::SceneManager::RenewalScene()
{
	if(currentScene!=nullptr) 
		currentScene->SceneEnd();
	currentScene = newScene;
	newScene = nullptr;
	currentScene->Set();
	currentScene->Start();
}

std::weak_ptr<ButiEngine::IApplication> ButiEngine::SceneManager::GetApplication()
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



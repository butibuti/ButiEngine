#include "stdafx.h"
#include "..\..\Header\GameParts\GameObjectManager.h"




ButiEngine::GameObjectManager::GameObjectManager(std::weak_ptr<IScene> arg_wkp_scene)
{
	wkp_scene = arg_wkp_scene;
}

void ButiEngine::GameObjectManager::Update()
{
	auto  endNewItr= vec_newGameObjects.end();
	for (auto itr = vec_newGameObjects.begin(); itr !=endNewItr; itr++) {
		vec_gameObjects.push_back(*itr);
		(*itr)->Start();
	}
	vec_newGameObjects.clear();
	auto itr = vec_gameObjects.begin();
	auto endItr = vec_gameObjects.end();
	while (itr !=endItr) {
		if ((*itr)->GetIsRemove()) {
			(*itr)->Release();
			itr = vec_gameObjects.erase(itr);
			endItr = vec_gameObjects.end();
		}
		else {
			(*itr)->Update();
			itr++;
		}
	}

}

void ButiEngine::GameObjectManager::Initialize()
{
}

void ButiEngine::GameObjectManager::PreInitialize()
{
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::AddObject(std::shared_ptr<GameObject> arg_gameObject)
{
	if (!map_gameObjects.count(arg_gameObject->GetObjectName())) {
		map_gameObjects.emplace(arg_gameObject->GetObjectName(), arg_gameObject);
		vec_newGameObjects.push_back(arg_gameObject);
		arg_gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}
	else {
		UINT count = 1;
		while (!map_gameObjects.count(arg_gameObject->GetObjectName()+"_"+std::to_string( count))) {
			count++;
		}
		arg_gameObject->SetObjectName(arg_gameObject->GetObjectName() + "_" + std::to_string(count));

		map_gameObjects.emplace(arg_gameObject->GetObjectName(), arg_gameObject);
		vec_newGameObjects.push_back(arg_gameObject);
		arg_gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}

	return arg_gameObject;
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::AddObject(std::shared_ptr<Transform> arg_transform,  std::string arg_objectName)
{
	std::shared_ptr<GameObject> out;
	if (!map_gameObjects.count(arg_objectName)) {

		out = ObjectFactory::Create<GameObject>(arg_transform, arg_objectName);
		vec_newGameObjects.push_back(out);
		map_gameObjects.emplace(arg_objectName,out);
		out->SetGameObjectManager(GetThis<GameObjectManager>());

	}
	else {
		UINT count = 1;
		while (!map_gameObjects.count(arg_objectName + "_" + std::to_string(count))) {
			count++;
		}
		arg_objectName += "_" + std::to_string(count);
		out = ObjectFactory::Create<GameObject>(arg_transform, arg_objectName);
		vec_newGameObjects.push_back(out);
		map_gameObjects.emplace(arg_objectName, out);
		out->SetGameObjectManager(GetThis<GameObjectManager>());
	}
	return out;
}


std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::GetGameObject(const std::string& arg_objectName)
{

	if (map_gameObjects.count(arg_objectName)) {
		return map_gameObjects.at(arg_objectName);
	}
	return std::weak_ptr<GameObject>();
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::GetGameObject(const GameObjectTag& arg_objectTag)
{
	auto output = std::weak_ptr<GameObject>();
	for (auto itr = vec_gameObjects.begin(); itr != vec_gameObjects.end(); itr++) {
		if ((*itr)->GetGameObjectTag() == arg_objectTag) {
			output = (*itr);
		}
	}
	return output;
}


std::vector<std::weak_ptr<ButiEngine::GameObject>> ButiEngine::GameObjectManager::GetGameObjects(const GameObjectTag& arg_objectTag)
{
	auto output = std::vector<std::weak_ptr<GameObject>>();
	for (auto itr = vec_gameObjects.begin(); itr != vec_gameObjects.end(); itr++) {
		if ((*itr)->GetGameObjectTag() == arg_objectTag) {
			output.push_back(*itr);
		}
	}
	return output;
}

void ButiEngine::GameObjectManager::UnRegistGameObject(std::shared_ptr<GameObject> arg_gameObject)
{
	if (map_gameObjects.count(arg_gameObject->GetObjectName()))
		map_gameObjects.erase(arg_gameObject->GetObjectName());
}

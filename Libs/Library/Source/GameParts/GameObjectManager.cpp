#include "stdafx.h"
#include "..\..\Header\GameParts\GameObjectManager.h"




ButiEngine::GameObjectManager::GameObjectManager(std::weak_ptr<IScene> arg_wkp_scene)
{
	wkp_scene = arg_wkp_scene;
}

void ButiEngine::GameObjectManager::Update()
{
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

void ButiEngine::GameObjectManager::RegistNewGameObject()
{
	auto  endNewItr = vec_newGameObjects.end();
	for (auto itr = vec_newGameObjects.begin(); itr != endNewItr; itr++) {
		vec_gameObjects.push_back(*itr);
		(*itr)->Start();
	}
	vec_newGameObjects.clear();
}

void ButiEngine::GameObjectManager::Initialize()
{
}

void ButiEngine::GameObjectManager::PreInitialize()
{
}

void ButiEngine::GameObjectManager::ShowUI()
{
	ImGui::Begin("hieralchy");

	auto endItr = vec_gameObjects.end();



	for (auto itr = vec_gameObjects.begin(); itr != vec_gameObjects.end(); itr++) {
		if (ImGui::Button((*itr)->GetGameObjectName().c_str())) {
			selectedGameObject = (*itr);
		}
	}

	ImGui::End();
	ImGui::Begin("SelectedObj");
	if (selectedGameObject.lock()) {

		selectedGameObject.lock()->ShowUI();

		if (ImGui::Button("Save!", ImVec2(200, 30))) {
			OutputCereal(selectedGameObject.lock());
		}
	}

	

	ImGui::End();
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

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::AddObjectFromCereal(std::string filePath, std::shared_ptr<Transform> arg_transform)
{
	auto gameObject = ObjectFactory::CreateFromCereal<GameObject>(filePath);

	if (arg_transform) {
		gameObject->transform = arg_transform;
	}

	if (!map_gameObjects.count(gameObject->GetGameObjectName())) {
		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_newGameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}
	else {
		UINT count = 1;
		while (!map_gameObjects.count(gameObject->GetGameObjectName() + "_" + std::to_string(count))) {
			count++;
		}
		gameObject->SetObjectName(gameObject->GetGameObjectName() + "_" + std::to_string(count));

		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_newGameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}

	return gameObject;
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

void ButiEngine::GameObjectManager::UnRegistGameObject(std::shared_ptr<GameObject> gameObject)
{
	if (map_gameObjects.count(gameObject->GetGameObjectName()))
		map_gameObjects.erase(gameObject->GetGameObjectName());
}

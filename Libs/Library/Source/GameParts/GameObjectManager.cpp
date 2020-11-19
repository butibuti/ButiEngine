#include "stdafx.h"
#include "..\..\Header\GameParts\GameObjectManager.h"


ButiEngine::GameObjectManager::GameObjectManager(std::weak_ptr<IScene> arg_wkp_scene)
{
	wkp_scene = arg_wkp_scene;
}

void ButiEngine::GameObjectManager::SetScene(std::weak_ptr<IScene> arg_wkp_scene)
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
	for (int i=0; i<vec_newGameObjects.size(); i++) {
		vec_gameObjects.push_back(vec_newGameObjects.at(i));
		vec_newGameObjects.at(i)->Start();
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
	ImGui::Begin("Hieralchy");

	std::shared_ptr<GameObject> shp_dragging = nullptr;


	for (auto itr = vec_gameObjects.begin(); itr != vec_gameObjects.end();) {
		if (ImGui::Button((*itr)->GetGameObjectName().c_str())) {
			selectedGameObject = (*itr);
		}

		if (ImGui::IsItemActive()) {
			ImGui::GetForegroundDrawList()->AddLine(GetApplication().lock()->GetGUIController()->GetGUIIO().MouseClickedPos[0], GetApplication().lock()->GetGUIController()->GetGUIIO().MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
			shp_dragging = *itr;
		}

		if (selectedGameObject.lock() == (*itr)) {

			ImGui::SameLine();
			if (ImGui::Button("..."))
				ImGui::OpenPopup("select_popup");
			if (ImGui::BeginPopup("select_popup"))
			{
				if (ImGui::Button("Dlete")) {
					(*itr)->Release();
					itr = vec_gameObjects.erase(itr);
				}
				else if(ImGui::Button("Copy")) {
					auto obj= AddObject((*itr)->Clone());


					obj.lock()->Init_RegistGameComponents();
					obj.lock()->Init_RegistBehaviors();

					itr++;
				}
				else {

					itr++;
				}
				ImGui::EndPopup();
			}
			else {
				itr++;
			}

			
		}
		else {
			itr++;
		}
	}

	if (ImGui::Button("Add New")) {
		AddObject(ObjectFactory::Create<Transform>());
	}

	GetApplication().lock()->GetGUIController()->SetDraggingObject(shp_dragging);

	ImGui::End();


}

void ButiEngine::GameObjectManager::Release()
{
	auto endItr = vec_gameObjects.end();
	for (auto itr = vec_gameObjects.begin(); itr != endItr; itr++) {
		(*itr)->Release();
	}
}

void ButiEngine::GameObjectManager::Initialize_cereal()
{

	for (auto itr = vec_gameObjects.begin(); itr != vec_gameObjects.end(); itr++) {
		(*itr)->Initialize();
		(*itr)->SetGameObjectManager(GetThis<GameObjectManager>());
		(*itr)->Init_RegistBehaviors();
		(*itr)->Init_RegistGameComponents();
	}
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::GetSelectedUI()
{
	return selectedGameObject;
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
		while (map_gameObjects.count(arg_objectName + "_" + std::to_string(count))) {
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

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::AddObject(std::shared_ptr<GameObject> gameObject)
{
	if (!map_gameObjects.count(gameObject->GetGameObjectName())) {
		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_newGameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}
	else {
		UINT count = 1;
		while (map_gameObjects.count(gameObject->GetGameObjectName() + "_" + std::to_string(count))) {
			count++;
		}
		gameObject->SetObjectName(gameObject->GetGameObjectName() + "_" + std::to_string(count));

		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_newGameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}


	return gameObject;
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::AddObjectFromCereal(std::string filePath, std::shared_ptr<Transform> arg_transform)
{

	auto gameObject = ObjectFactory::CreateFromCereal<GameObject>(GlobalSettings::GetResourceDirectory()+"GameObject/" +filePath+".gameObject")->Clone();
	
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
		while (map_gameObjects.count(gameObject->GetGameObjectName() + "_" + std::to_string(count))) {
			count++;
		}
		gameObject->SetObjectName(gameObject->GetGameObjectName() + "_" + std::to_string(count));

		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_newGameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}

	gameObject->Init_RegistGameComponents();
	gameObject->Init_RegistBehaviors();

	return gameObject;
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::GameObjectManager::AddObjectFromCereal_Insert(std::string filePath, std::shared_ptr<Transform> arg_transform)
{
	auto gameObject = ObjectFactory::CreateFromCereal<GameObject>(GlobalSettings::GetResourceDirectory() + "GameObject/" + filePath);

	if (arg_transform) {
		gameObject->transform = arg_transform;
	}

	if (!map_gameObjects.count(gameObject->GetGameObjectName())) {
		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_gameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}
	else {
		UINT count = 1;
		while (map_gameObjects.count(gameObject->GetGameObjectName() + "_" + std::to_string(count))) {
			count++;
		}
		gameObject->SetObjectName(gameObject->GetGameObjectName() + "_" + std::to_string(count));

		map_gameObjects.emplace(gameObject->GetGameObjectName(), gameObject);
		vec_gameObjects.push_back(gameObject);
		gameObject->SetGameObjectManager(GetThis<GameObjectManager>());
	}

	gameObject->Init_RegistGameComponents();
	gameObject->Init_RegistBehaviors();

	return gameObject;
}

std::string ButiEngine::GameObjectManager::ReNameGameObject(const std::string& arg_ObjectName, const std::string& arg_befObjectName)
{
	auto object = map_gameObjects.at(arg_befObjectName);
	map_gameObjects.erase(arg_befObjectName);

	auto nowName = arg_ObjectName;
	if (!map_gameObjects.count(nowName)) {

		map_gameObjects.emplace(nowName, object);

	}
	else {
		UINT count = 1;
		while (map_gameObjects.count(nowName + "_" + std::to_string(count))) {
			count++;
		}
		nowName += "_" + std::to_string(count);
		map_gameObjects.emplace(nowName, object);
	}
	return nowName;
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
	auto endItr = vec_gameObjects.end();
	for (auto itr = vec_gameObjects.begin(); itr != endItr; itr++) {
		if ((*itr)->GetGameObjectTag() == arg_objectTag) {
			output = (*itr);
		}
	}
	return output;
}


std::vector<std::weak_ptr<ButiEngine::GameObject>> ButiEngine::GameObjectManager::GetGameObjects(const GameObjectTag& arg_objectTag)
{
	auto output = std::vector<std::weak_ptr<GameObject>>();
	auto endItr = vec_gameObjects.end();
	for (auto itr = vec_gameObjects.begin(); itr !=endItr; itr++) {
		if ((*itr)->GetGameObjectTag() == arg_objectTag) {
			output.push_back(*itr);
		}
	}
	return output;
}

void ButiEngine::GameObjectManager::Start()
{
	auto endItr = vec_gameObjects.end();
	for (auto itr = vec_gameObjects.begin(); itr != endItr; itr++) {
		(*itr)->Start();
	}
}

std::weak_ptr< ButiEngine::Application> ButiEngine::GameObjectManager::GetApplication()
{
	return wkp_scene.lock()->GetSceneManager().lock()->GetApplication().lock();
}

void ButiEngine::GameObjectManager::UnRegistGameObject(std::shared_ptr<GameObject> gameObject)
{
	if (map_gameObjects.count(gameObject->GetGameObjectName()))
		map_gameObjects.erase(gameObject->GetGameObjectName());
}


void ButiEngine::OutputCereal(const std::shared_ptr<GameObjectManager>& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<GameObjectManager>& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

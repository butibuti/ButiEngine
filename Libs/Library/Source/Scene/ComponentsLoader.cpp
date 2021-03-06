#pragma once
#include "stdafx.h"
#include "..\..\Header\Scene\ComponentsLoader.h"
#include "..\..\Header\Common\CerealUtill.h"

std::shared_ptr<  ButiEngine::ComponentsLoader> ButiEngine::ComponentsLoader::instance = nullptr;

void ButiEngine::ComponentsLoader::CreateInstance()
{
	if (instance)
		return;

	instance= ObjectFactory::CreateFromCereal<ComponentsLoader>(GlobalSettings::GetResourceDirectory() + "Application/componentLoader.loader");
	instance->CreateNameList();
}

void ButiEngine::ComponentsLoader::SaveInstance()
{
	OutputCereal(instance, GlobalSettings::GetResourceDirectory() + "Application/componentLoader.loader");
}

std::shared_ptr<ButiEngine::ComponentsLoader> ButiEngine::ComponentsLoader::GetInstance()
{
	return instance;
}

void ButiEngine::ComponentsLoader::Release()
{

	ComponentsLoader::GetInstance()->SaveInstance();
	for (int i = 0; i < componentNameListSize; i++) {
		delete componentNameList[i];
	}
	delete componentNameList;

	addGameComponents.clear();
}

void ButiEngine::ComponentsLoader::CreateNameList()
{
	if (componentNameList) {
		delete componentNameList;
	}

	componentNameListSize = addGameComponents.size();
	componentNameList = (char**)malloc(sizeof(char*) * componentNameListSize);

	for (int i = 0; i < componentNameListSize; i++) {
		auto name = addGameComponents.at(i)->GetGameComponentName();
		int size = name.size();
		componentNameList[i] = (char*)malloc(size * sizeof(char) + 1);
		auto name_c_str = name.c_str();
		strcpy_s(componentNameList[i], size + 1, name_c_str);
	}

}

void ButiEngine::ComponentsLoader::AddGameComponent(std::shared_ptr<GameComponent> arg_gamecomponent)
{
	bool isContain = false;
	auto enditr = addGameComponents.end();
	for (auto itr = addGameComponents.begin(); itr != enditr; itr++) {
		if ((*itr)->GetGameComponentName() == arg_gamecomponent->GetGameComponentName()) {
			isContain = true;
			(*itr) = arg_gamecomponent;
		}
	}
	if (!isContain)
		addGameComponents.push_back(arg_gamecomponent);
}

void ButiEngine::ComponentsLoader::RemoveComponent(const std::string& arg_name)
{
	for (auto itr = addGameComponents.begin(); itr != addGameComponents.end(); itr++) {
		if (arg_name == (*itr)->GetGameComponentName()) {
			addGameComponents.erase(itr);
			break;
		}
	}
}


std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ComponentsLoader::ShowAddGameComponentUI()
{
	std::shared_ptr<GameComponent> ret=nullptr;

	if (GUI::TreeNode("Add GameComponent")) {
		if (GUI::ListBox("##GameComponents", currentIndex_componentList, componentNameList, componentNameListSize, 5)) {
		}


		if (GUI::Button("Add!")) {
			ret= addGameComponents.at(currentIndex_componentList)->Clone();
			
		}

		GUI::TreePop();
	}
	return ret;
}

void ButiEngine::OutputCereal(const std::shared_ptr<ComponentsLoader>& v,const std::string& path )
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<ComponentsLoader>& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}
#pragma once
#include "stdafx.h"
#include "..\..\Header\Scene\ComponentsLoader.h"

void ButiEngine::ComponentsLoader::Release()
{

	for (int i = 0; i < componentNameListSize; i++) {
		delete componentNameList[i];
	}
	delete componentNameList;

	for (int i = 0; i < behaviorNameListSize; i++) {
		delete behaviorNameList[i];
	}
	delete behaviorNameList;
	addBehaviors.clear();
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

	if (behaviorNameList) {
		delete behaviorNameList;
	}
	behaviorNameListSize = addBehaviors.size();

	behaviorNameList = (char**)malloc(sizeof(char*) * behaviorNameListSize);

	for (int i = 0; i < behaviorNameListSize; i++) {
		auto name = addBehaviors.at(i)->GetBehaviorName();
		int size = name.size();
		behaviorNameList[i] = (char*)malloc(size * sizeof(char) + 1);
		auto name_c_str = name.c_str();
		strcpy_s(behaviorNameList[i], size + 1, name_c_str);
	}
}

void ButiEngine::ComponentsLoader::AddBehavior(std::shared_ptr<Behavior> arg_behavior)
{
	bool isContain = false;
	auto enditr = addBehaviors.end();
	for (auto itr = addBehaviors.begin(); itr != enditr; itr++) {
		if ((*itr)->GetBehaviorName() == arg_behavior->GetBehaviorName()) {
			isContain = true;
			(*itr) = arg_behavior;
		}
	}
	if(!isContain)
	addBehaviors.push_back(arg_behavior);
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

void ButiEngine::ComponentsLoader::RemoveBehavior(const std::string& arg_name)
{
	for (auto itr = addBehaviors.begin(); itr != addBehaviors.end(); itr++) {
		if (arg_name == (*itr)->GetBehaviorName()) {
			addBehaviors.erase(itr);
			break;
		}
	}
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::ComponentsLoader::ShowAddBehaviorUI()
{
	std::shared_ptr<Behavior> ret = nullptr;
	if (ImGui::TreeNode("Add Behavior")) {


		if (ImGui::ListBox("##Behaviors", &currentIndex_behaviorList, behaviorNameList, behaviorNameListSize, 5)) {
		}

		if (ImGui::Button("Add!")) {
			ret = addBehaviors.at(currentIndex_behaviorList)->Clone();
		}

		ImGui::TreePop();
	}
	return ret;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ComponentsLoader::ShowAddGameComponentUI()
{
	std::shared_ptr<GameComponent> ret=nullptr;

	if (ImGui::TreeNode("Add GameComponent")) {
		if (ImGui::ListBox("##GameComponents", &currentIndex_componentList, componentNameList, componentNameListSize, 5)) {
		}


		if (ImGui::Button("Add!")) {
			ret= addGameComponents.at(currentIndex_componentList)->Clone();
			
		}

		ImGui::TreePop();
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
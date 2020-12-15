#include"stdafx.h"
#include "..\..\Header\GameObjects\GameObjectTagManager.h"


ButiEngine::GameObjectTag ButiEngine::GameObjectTagManager::CreateGameObjectTag(std::string arg_tagName)
{
	return idManager.AddTag(arg_tagName);
}

ButiEngine::GameObjectTag ButiEngine::GameObjectTagManager::GetObjectTag(std::string arg_tagName)
{
	return idManager.GetTag(arg_tagName);
}

std::string ButiEngine::GameObjectTagManager::GetTagName(const GameObjectTag& arg_gameObjtag)
{
	return idManager.GetIDName(arg_gameObjtag);
}

void ButiEngine::OutputCereal(const std::shared_ptr<GameObjectTagManager>& v, const std::string& arg_filePath)
{
	std::stringstream stream;


	cereal::PortableBinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + arg_filePath, std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<GameObjectTagManager>& v, const std::string& arg_filePath)
{
	std::stringstream stream;

	std::string path = GlobalSettings::GetResourceDirectory() + arg_filePath;

	if (!Util::IsFileExistence(path)) {
		return;
	}

	std::ifstream inputFile(GlobalSettings::GetResourceDirectory() + arg_filePath, std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::PortableBinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

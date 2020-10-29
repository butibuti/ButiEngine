#include"stdafx.h"

ButiEngine::IDManager<ButiEngine::GameObject> ButiEngine::GameObjectTagManager::idManager;

ButiEngine::GameObjectTag ButiEngine::GameObjectTagManager::CreateGameObjectTag(std::string arg_tagName)
{
	return idManager.AddTag(arg_tagName);
}

ButiEngine::GameObjectTag ButiEngine::GameObjectTagManager::GetObjectTag(std::string arg_tagName)
{
	return idManager.GetTag(arg_tagName);
}

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

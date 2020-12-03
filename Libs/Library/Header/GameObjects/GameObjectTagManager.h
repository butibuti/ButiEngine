#pragma once
#include "stdafx.h"
namespace ButiEngine {
	class GameObject;
	using GameObjectTag= ID<GameObject>;

	class GameObjectTagManager
	{
	public:
		GameObjectTag CreateGameObjectTag(std::string arg_tagName);
		GameObjectTag GetObjectTag(std::string arg_tagName);
		std::string GetTagName(const GameObjectTag& arg_gameObj);
	private:
		IDManager<GameObject> idManager;
	};

}
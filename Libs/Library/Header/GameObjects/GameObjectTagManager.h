#pragma once
#include "stdafx.h"
namespace ButiEngine {
	using GameObjectTag= ID<GameObject>;

	class GameObjectTagManager
	{
	public:
		static GameObjectTag CreateGameObjectTag(std::string arg_tagName);
		static GameObjectTag GetObjectTag(std::string arg_tagName);
	private:
		static IDManager<GameObject> idManager;
	};

}
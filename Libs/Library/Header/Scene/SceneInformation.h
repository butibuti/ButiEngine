#pragma once
#include "stdafx.h"
namespace ButiEngine {

	struct SceneInformation
	{
	public:
		SceneInformation(const std::string& arg_sceneName) {
			sceneName = arg_sceneName;
		}
		~SceneInformation();
		std::string GetSceneName() {
			return sceneName;
		}
	private:
		std::string sceneName;
	};
}


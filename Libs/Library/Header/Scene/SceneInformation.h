#pragma once
#include "stdafx.h"
namespace ButiEngine {

	struct SceneInformation:public IObject
	{
	public:
		SceneInformation(const std::string& arg_sceneName) {
			sceneName = arg_sceneName;
		}
		~SceneInformation();
		std::string GetSceneName() {
			return sceneName;
		}
		void Initialize()override{}
		void PreInitialize()override{}
	private:
		std::string sceneName;
	};
}


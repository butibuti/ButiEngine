#pragma once
#include "Header/Scene/Scene.h"
#include "Header/Common/CBuffer_Dx12.h"

namespace ButiEngine {

	class SampleScene :public Scene
	{
	public:
		SampleScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation sceneInformation);
		~SampleScene();
		void OnUpdate() override;
		void OnInitialize()override;
		void PreInitialize()override;
		void OnSceneEnd()override;
		std::weak_ptr< CBuffer_Dx12< TestGSVariable>> shp_testGSVariable;
		float t = 0;
		int dir = 1;
	};
}

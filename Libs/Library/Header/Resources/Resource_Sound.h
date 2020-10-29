#pragma once
#include"stdafx.h"
#include"WaveDatas.h"
namespace ButiEngine {
	class Resource_Sound :public IResource_Sound
	{
	public:
		void Initialize()override;
		void PreInitialize()override;
		std::shared_ptr<WaveDatas> GetWavDatas();
	protected:
		std::shared_ptr<WaveDatas> shp_wavData;
	};

}
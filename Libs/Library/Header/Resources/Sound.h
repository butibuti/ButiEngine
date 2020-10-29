#pragma once
#include"stdafx.h"
#include"WaveDatas.h"
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

namespace ButiEngine {

	class SoundHelper
	{
	public:
		~SoundHelper();

		static std::shared_ptr<WaveDatas> CreateFromFile(const std::string& filePath);
	private:

		SoundHelper();
	};
}


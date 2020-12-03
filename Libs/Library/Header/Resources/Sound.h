#pragma once
#include"stdafx.h"
#include"WaveDatas.h"

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


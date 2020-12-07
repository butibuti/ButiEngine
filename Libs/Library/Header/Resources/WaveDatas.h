#pragma once
#include"../Device/ObjectFactory.h"
#include <xaudio2.h>	
namespace ButiEngine {

	struct WaveDatas:public IObject
	{
	public:
		WaveDatas(const XAUDIO2_BUFFER& arg_buffer, const WAVEFORMATEX& format);
		~WaveDatas();
		void Initialize()override{}
		void PreInitialize()override{}
		XAUDIO2_BUFFER buffer;
		WAVEFORMATEX format;
	};
}


#pragma once
#include"stdafx.h"
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

namespace ButiEngine {

	class GameDevice
	{
	public:
		GameDevice();
		~GameDevice();
		static Input input;
		static float WorldSpeed;
	};
}

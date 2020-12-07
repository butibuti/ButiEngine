#pragma once

#include"../Device/Input.h"
namespace ButiEngine {

	struct BlankSpace {
		UINT index;
		UINT size;
	};
	class GameDevice
	{
	public:
		~GameDevice();
		static void Initialize();
		static float WorldSpeed;
		static std::unique_ptr<Input>& GetInput();
	private:
		GameDevice();
		static std::unique_ptr<Input> input;
	};
}

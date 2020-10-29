#pragma once
#include <random>
#include <memory>
namespace ButiEngine {

	class RandomMaker
	{
	public:
		RandomMaker();
		~RandomMaker();
		static float GetValue(float min, float max, int pase);
		static std::shared_ptr< std::random_device> shp_rnd_device;
		static std::shared_ptr<std::mt19937>shp_mt;
		static std::shared_ptr< std::uniform_int_distribution<>> shp_randRange;
	};
}


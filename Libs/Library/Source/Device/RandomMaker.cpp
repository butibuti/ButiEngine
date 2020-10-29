#include"stdafx.h"
#include "../../Header/Device/RandomMaker.h"
using namespace::ButiEngine;



RandomMaker::RandomMaker()
{
	
}

 std::shared_ptr< std::random_device> RandomMaker::shp_rnd_device = std::make_shared<std::random_device>();
 std::shared_ptr<std::mt19937> RandomMaker::shp_mt = std::make_shared<std::mt19937>((*shp_rnd_device)());
 std::shared_ptr<std::uniform_int_distribution<>> RandomMaker::shp_randRange= std::make_shared< std::uniform_int_distribution<>>(0, 1);

RandomMaker::~RandomMaker()
{
}

float RandomMaker::GetValue(float min, float max,int pase)
{	
	return 0;
}


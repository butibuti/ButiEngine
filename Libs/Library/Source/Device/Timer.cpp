#include"stdafx.h"


using namespace::ButiEngine;


Timer::Timer(float arg_maxCountframe)
{
	maxCountFrame = arg_maxCountframe;
	nowCountFrame = 0;
	isOn = false;
}

bool Timer::IsOn()
{
	return isOn;
}

void Timer::Start()
{
	isOn = true;
}

void Timer::Stop()
{
	isOn = false;
}

void Timer::Initialize()
{
	nowCountFrame = 0;
	isOn = false;
}

void ButiEngine::Timer::PreInitialize()
{
}

void ButiEngine::Timer::SetCount(int arg_nowCount)
{
	nowCountFrame = arg_nowCount;
}

void ButiEngine::Timer::ChangeCountFrame(int arg_maxCount)
{
	maxCountFrame = arg_maxCount;
}

void ButiEngine::Timer::Reset()
{
	nowCountFrame = 0;
}


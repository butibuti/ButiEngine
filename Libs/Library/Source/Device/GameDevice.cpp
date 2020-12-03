#include"stdafx.h"



ButiEngine::GameDevice::GameDevice()
{
}


std::unique_ptr<ButiEngine::Input> ButiEngine::GameDevice::input = nullptr;
//SoundManager* GameDevice::soundManager =new SoundManager();
float ButiEngine::GameDevice::WorldSpeed = 1.0f;

ButiEngine::GameDevice::~GameDevice()
{
}

void ButiEngine::GameDevice::Initialize()
{
	input = std::make_unique<Input>();
}

std::unique_ptr<ButiEngine::Input>& ButiEngine::GameDevice::GetInput()
{
	return input;
}

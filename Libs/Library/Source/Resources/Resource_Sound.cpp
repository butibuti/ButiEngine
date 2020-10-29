#include"stdafx.h"

#include "..\..\Header\Resources\Resource_Sound.h"
void ButiEngine::Resource_Sound::Initialize()
{
}

void ButiEngine::Resource_Sound::PreInitialize()
{
}

std::shared_ptr<ButiEngine::WaveDatas> ButiEngine::Resource_Sound::GetWavDatas()
{
    return shp_wavData;
}

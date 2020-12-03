#include "stdafx.h"
#include "..\..\Header\Resources\Resource_Sound_XAudio2.h"
#include"Header/Resources/Sound.h"


ButiEngine::Resource_Sound_XAudio2::Resource_Sound_XAudio2(const std::string& arg_filePath)
{
	shp_wavData = SoundHelper::CreateFromFile(arg_filePath);
}


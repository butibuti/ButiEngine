#include"stdafx.h"
#include"Header/Resources/WaveDatas.h"
using namespace::ButiEngine;

WaveDatas::WaveDatas(const XAUDIO2_BUFFER& arg_buffer, const WAVEFORMATEX& arg_format)
{
	buffer = arg_buffer;
	format = arg_format;
}

WaveDatas::~WaveDatas()
{
	delete buffer.pAudioData;
}

#include"stdafx.h"
#include"Header/Resources/Sound.h"
#include"Header/Resources/WaveDatas.h"


#include <xaudio2.h>	
#include <xaudio2fx.h>
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "xaudio2.lib" )
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
using namespace::ButiEngine;

SoundHelper::SoundHelper()
{

}


SoundHelper::~SoundHelper()
{
}
#ifdef _XBOX //Big-Endian#define fourccRIFF 'RIFF'#define fourccDATA 'data'#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'#endif#ifndef _XBOX //Little-Endian#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'#define fourccXWMA 'AMWX'#define fourccDPDS 'sdpd'
#endif
struct Chunk
{
	char	id[4]; // �`�����N����ID
	int32_t	size;  // �`�����N�T�C�Y
};

// RIFF�w�b�_�[
struct RiffHeader
{
	Chunk	chunk;   // "RIFF"
	char	type[4]; // "WAVE"
};

// FMT�`�����N
struct FormatChunk
{
	Chunk		chunk; // "fmt "
	WAVEFORMAT	fmt;   // �g�`�t�H�[�}�b�g
};
std::shared_ptr<WaveDatas> SoundHelper::CreateFromFile(const std::string& filePath)
{
	//auto outsound = std::make_shared<Sound>();
	XAUDIO2_BUFFER buffer = { 0 };

	HRESULT hr;

#ifdef _XBOX
	char * strFileName = "game:\\media\\filePath";
#else
#endif
	// Open the file
	BinaryReader reader = BinaryReader();

	if(Util::CheckFileExistence)

	reader.ReadStart(filePath);

	// RIFF�w�b�_�[�̓ǂݍ���
	auto riff=reader.ReadVariable<RiffHeader>();

	// Format�`�����N�̓ǂݍ���
	auto format = reader.ReadVariable<FormatChunk>();

	// Data�`�����N�̓ǂݍ���
	reader.Skip("data");
	auto data = reader.ReadVariable<Chunk>();

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer=(char*)reader.ReadData( data.size);
	reader.ReadEnd();

	WAVEFORMATEX wfex{};
	// �g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	buffer.pAudioData = (BYTE*)pBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = data.size;
	
	return ObjectFactory::Create<WaveDatas>(buffer, wfex);
}

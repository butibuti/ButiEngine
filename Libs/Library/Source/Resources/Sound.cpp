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
	char	id[4]; // チャンク毎のID
	int32_t	size;  // チャンクサイズ
};

// RIFFヘッダー
struct RiffHeader
{
	Chunk	chunk;   // "RIFF"
	char	type[4]; // "WAVE"
};

// FMTチャンク
struct FormatChunk
{
	Chunk		chunk; // "fmt "
	WAVEFORMAT	fmt;   // 波形フォーマット
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

	// RIFFヘッダーの読み込み
	auto riff=reader.ReadVariable<RiffHeader>();

	// Formatチャンクの読み込み
	auto format = reader.ReadVariable<FormatChunk>();

	// Dataチャンクの読み込み
	reader.Skip("data");
	auto data = reader.ReadVariable<Chunk>();

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer=(char*)reader.ReadData( data.size);
	reader.ReadEnd();

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	buffer.pAudioData = (BYTE*)pBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = data.size;
	
	return ObjectFactory::Create<WaveDatas>(buffer, wfex);
}

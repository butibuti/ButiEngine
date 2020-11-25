#pragma once
#include"stdafx.h"

#include"Header/Resources/Sound.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Resources/Resource_Sound.h"
#include "..\..\Header\GameParts\SoundManager.h"
ButiEngine::SoundManager::SoundManager(std::weak_ptr<IScene> arg_wkp_iscene)
{
	wkp_iscene = arg_wkp_iscene;
}
ButiEngine::SoundManager::~SoundManager()
{
}

void ButiEngine::SoundManager::Initialize()
{
	CoInitialize(NULL);
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(cmp_pXAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR)))

		throw ButiException(L"Failed", L"Create", L"XAudio2");

	if (FAILED(hr = cmp_pXAudio2->CreateMasteringVoice(&p_masterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL)))

		throw ButiException(L"Failed", L"Create", L"MasteringVoice");
	shp_clearCheckTimer = ObjectFactory::Create<RelativeTimer>(600);
	shp_clearCheckTimer->Start();
}

void ButiEngine::SoundManager::StopCheck()
{

}

void ButiEngine::SoundManager::Update()
{
	if (shp_clearCheckTimer->Update()) {
		ClearCheck();
	}
}

void ButiEngine::SoundManager::ClearCheck()
{
	auto itr = vec_seVoices.begin();
	while (itr != vec_seVoices.end()) {
		XAUDIO2_VOICE_STATE state;
		(*itr)->GetState(&state);
		if (!state.BuffersQueued) {
			(*itr)->DestroyVoice();
			itr = vec_seVoices.erase(itr);
		}
		else {
			itr++;
		}
	}
}



void ButiEngine::SoundManager::Play(SoundTag tag, float valume)
{
	IXAudio2SourceVoice* pSourceVoice;

	//waveDataPtrs.at(tag)->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	auto waveData = wkp_iscene.lock()->GetSceneManager().lock()->GetApplication().lock()->GetResourceContainer()->GetSound(tag).lock()->GetWavDatas();
	waveData->buffer.Flags = XAUDIO2_END_OF_STREAM;
	HRESULT hr = cmp_pXAudio2->CreateSourceVoice(&pSourceVoice, &waveData->format);
	hr = pSourceVoice->SubmitSourceBuffer(&(waveData->buffer));
	pSourceVoice->SetVolume(valume*4);
	hr = pSourceVoice->Start();
	vec_seVoices.push_back(pSourceVoice);

}

void ButiEngine::SoundManager::PlayBGM(SoundTag tag, float valume)
{
	if (cmp_bgm) {
		cmp_bgm->Stop();
		cmp_bgm->DestroyVoice();
	}
	auto waveData = wkp_iscene.lock()->GetSceneManager().lock()->GetApplication().lock()->GetResourceContainer()->GetSound(tag).lock()->GetWavDatas();
	waveData->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	waveData->buffer.Flags = XAUDIO2_END_OF_STREAM;
	HRESULT hr = cmp_pXAudio2->CreateSourceVoice(&cmp_bgm, &waveData->format);
	cmp_bgm->SubmitSourceBuffer(&waveData->buffer);
	cmp_bgm->SetVolume(valume);
	cmp_bgm->Start();
}

void ButiEngine::SoundManager::Release()
{
	for (auto itr = vec_seVoices.begin(); itr != vec_seVoices.end(); itr++) {
		(*itr)->DestroyVoice();
	}
	if (cmp_bgm) {
		cmp_bgm->Stop();
		cmp_bgm->DestroyVoice();
	}
	p_masterVoice->DestroyVoice();
}
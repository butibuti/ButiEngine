#pragma once
#include"stdafx.h"
#include"../Resources/WaveDatas.h"
namespace ButiEngine {
	class SoundManager:public ISoundManager
	{
	public:
		SoundManager(std::weak_ptr<IApplication> arg_wkp_app);
		~SoundManager();
		void Initialize()override;
		void StopCheck()override;
		void Update()override;
		void ClearCheck()override;
		void PlaySE(SoundTag tag, float volume)override;
		void PlayBGM(SoundTag tag,float volume)override;
		void Release()override;
		void StopSE() override;
		void StopBGM() override;
		void RestartSE() override;
		void RestartBGM() override;
		void DestroySE() override;
		void DestroyBGM() override;
		void SetBGMVolume(float volume) override;
		SoundTag GetNowPlayBGM()override;
	private:
		SoundTag nowBGM;
		IXAudio2SourceVoice* cmp_bgm;
		IXAudio2MasteringVoice* p_masterVoice;
		Microsoft::WRL::ComPtr< IXAudio2> cmp_pXAudio2;
		std::vector<IXAudio2SourceVoice*> vec_seVoices;
		std::shared_ptr<Timer> shp_clearCheckTimer; 
		std::weak_ptr<IApplication> wkp_app;
	};
}


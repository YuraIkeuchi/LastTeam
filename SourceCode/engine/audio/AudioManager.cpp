#include "AudioManager.h"
#include "Audio.h"


AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;

	return &instance;
}

//�񓯊��O�ɓǂݍ��ނ���
void AudioManager::StartLoadAudio() {
	Audio::GetInstance()->LoadSound(AUDIO_MAIN, "Resources/Sound/BGM/mainBGM.wav");

}

void AudioManager::SecondLoadAudio() {
	//Audio::GetInstance()->LoadSound(AUDIO_BATTLE, "Resources/Sound/BGM/BGM_boss.wav");
	//Audio::GetInstance()->LoadSound(AUDIO_BATTLE2, "Resources/Sound/BGM/gamePlayBack2.wav");
}
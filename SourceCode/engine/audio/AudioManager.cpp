#include "AudioManager.h"
#include "Audio.h"


AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;

	return &instance;
}

//”ñ“¯Šú‘O‚É“Ç‚Ýž‚Þ‚à‚Ì
void AudioManager::StartLoadAudio() {
	//Audio::GetInstance()->LoadSound(AUDIO_TITLE, "Resources/Sound/BGM/BGM_title.wav");
	//Audio::GetInstance()->LoadSound(AUDIO_LOAD, "Resources/Sound/BGM/BGM_load.wav");
}

void AudioManager::SecondLoadAudio() {
	//Audio::GetInstance()->LoadSound(AUDIO_BATTLE, "Resources/Sound/BGM/BGM_boss.wav");
	//Audio::GetInstance()->LoadSound(AUDIO_BATTLE2, "Resources/Sound/BGM/gamePlayBack2.wav");
}
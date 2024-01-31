#include "AudioManager.h"
#include "Audio.h"


AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;

	return &instance;
}

//”ñ“¯Šú‘O‚É“Ç‚Ýž‚Þ‚à‚Ì
void AudioManager::StartLoadAudio() {
	Audio::GetInstance()->LoadSound(AUDIO_MAIN, "Resources/Sound/BGM/mainBGM.wav");
	Audio::GetInstance()->LoadSound(AUDIO_OVER, "Resources/Sound/BGM/overBGM.wav");

}

void AudioManager::SecondLoadAudio() {
	//Audio::GetInstance()->LoadSound(AUDIO_BATTLE, "Resources/Sound/BGM/BGM_boss.wav");
	//Audio::GetInstance()->LoadSound(AUDIO_BATTLE2, "Resources/Sound/BGM/gamePlayBack2.wav");
}
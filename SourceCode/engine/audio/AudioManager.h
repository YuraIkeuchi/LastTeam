#pragma once

//オーディオの読み込みクラス
class AudioManager {
public:
	static AudioManager* GetInstance();

	void StartLoadAudio();
	void SecondLoadAudio();
};
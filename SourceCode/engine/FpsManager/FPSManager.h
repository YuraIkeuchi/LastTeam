#pragma once
#include<Windows.h>
#include<tchar.h>
#include<sstream>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")



class FPSManager
{
public:
	const float& GetFps() { return  fps; }
public:
	static const int FRAME_RATE = 60;
	const float MIN_FREAM_TIME = 1.0f / FRAME_RATE;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;

	float fps = 0;
	static FPSManager* GetInstance() {
		static FPSManager instance;
		return &instance;
	}

	//èâä˙âª
	void Init();

	//FPSí≤êÆ
	bool Run();

	void ImGuiDraw();
};

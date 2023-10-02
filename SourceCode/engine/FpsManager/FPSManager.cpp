#include"FPSManager.h"
#include "imgui.h"


void FPSManager::Init() {
	if (QueryPerformanceFrequency(&timeFreq) == FALSE) {
	}

	QueryPerformanceCounter(&timeStart);
}

bool FPSManager::Run()
{
	// ¡‚ÌŠÔ‚ğæ“¾
	QueryPerformanceCounter(&timeEnd);
	// (¡‚ÌŠÔ - ‘OƒtƒŒ[ƒ€‚ÌŠÔ) / ü”g” = Œo‰ßŠÔ(•b’PˆÊ)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) { // ŠÔ‚É—]—T‚ª‚ ‚é
		// ƒ~ƒŠ•b‚É•ÏŠ·
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1); // •ª‰ğ”\‚ğã‚°‚é(‚±‚¤‚µ‚È‚¢‚ÆSleep‚Ì¸“x‚ÍƒKƒ^ƒKƒ^)
		Sleep(sleepTime);   // Q‚é
		timeEndPeriod(1);   // –ß‚·

		// ŸT‚É‚¿‰z‚µ(‚±‚¤‚µ‚È‚¢‚Æfps‚ª•Ï‚É‚È‚é?)
		return false;
	}

	if (frameTime > 0.0) { // Œo‰ßŠÔ‚ª0‚æ‚è‘å‚«‚¢(‚±‚¤‚µ‚È‚¢‚Æ‰º‚ÌŒvZ‚Åƒ[ƒœZ‚É‚È‚é‚Æv‚í‚ê)
		fps = (fps * 0.99f) + (0.01f / frameTime); // •½‹Ïfps‚ğŒvZ
		timeStart = timeEnd; // “ü‚ê‘Ö‚¦
		return true;
	}
	return true;
}

void FPSManager::ImGuiDraw() {
	ImGui::Begin("FPS");
	ImGui::Text("FPS : %f", fps);
	ImGui::End();
}

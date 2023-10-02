#include "GameMode.h"
#include "imgui.h"
GameMode* GameMode::GetInstance()
{
	static GameMode instance;

	return &instance;
}

bool GameMode::Initialize() {
	m_GameTurn = TURN_SET;
	return true;
}

//ImGui
void GameMode::ImGuiDraw() {
	ImGui::Begin("Mode");
	if (m_GameTurn == TURN_SET) {
		ImGui::Text("SET");
	}
	else {
		ImGui::Text("Battle");
	}
	ImGui::End();
}
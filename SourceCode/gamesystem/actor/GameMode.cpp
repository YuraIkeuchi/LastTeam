#include "GameMode.h"
#include "imgui.h"
#include "input.h"
GameMode* GameMode::GetInstance()
{
	static GameMode instance;

	return &instance;
}

bool GameMode::Initialize() {
	m_GameTurn = TURN_SET;
	return true;
}
//XV
void GameMode::Update() {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->LB))) {
		m_GameTurn = TURN_SET;
	}
	else if ((input->TriggerButton(input->RB))) {
		m_GameTurn = TURN_BATTLE;
	}
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
	if (ImGui::Button("SETMODE", ImVec2(50, 50))) {
		m_GameTurn = TURN_SET;
	}
	if (ImGui::Button("BUTTLEMODE", ImVec2(50, 50))) {
		m_GameTurn = TURN_BATTLE;
	}
	ImGui::End();
}
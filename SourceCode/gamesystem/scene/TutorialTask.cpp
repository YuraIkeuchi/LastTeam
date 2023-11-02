#include "TutorialTask.h"
#include "imgui.h"
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

//初期化
void  TutorialTask::Initialize() {
	m_TutorialState = TASK_MOVE;		//最初のタスクにする
}

//ImGui
void TutorialTask::ImGuiDraw() {
	ImGui::Begin("Task");
	ImGui::Text("Task:%d", m_TutorialState);
	ImGui::End();
}
#include "TutorialTask.h"
#include "imgui.h"
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

//‰Šú‰»
void  TutorialTask::Initialize() {
	m_TutorialState = TASK_MOVE;		//Å‰‚Ìƒ^ƒXƒN‚É‚·‚é
}

//ImGui
void TutorialTask::ImGuiDraw() {
	ImGui::Begin("Task");
	ImGui::Text("Task:%d", m_TutorialState);
	ImGui::End();
}
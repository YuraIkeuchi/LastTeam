#include "TutorialTask.h"
#include "imgui.h"
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

void TutorialTask::Initialize() {
	for (int i = 0; i < TASK_MAX; i++) {
		m_TaskFinish[i] = false;
	}
}
//ImGui
void TutorialTask::ImGuiDraw() {
	ImGui::Begin("Task");
	for (int i = 0; i < TASK_MAX; i++) {
		ImGui::Text("Task[%d]:%d",i, m_TaskFinish[i]);
	}
	ImGui::End();
}
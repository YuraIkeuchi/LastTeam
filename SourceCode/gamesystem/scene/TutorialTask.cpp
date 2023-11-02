#include "TutorialTask.h"
#include "imgui.h"
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

//������
void  TutorialTask::Initialize() {
	m_TutorialState = TASK_MOVE;		//�ŏ��̃^�X�N�ɂ���
}

//ImGui
void TutorialTask::ImGuiDraw() {
	ImGui::Begin("Task");
	ImGui::Text("Task:%d", m_TutorialState);
	ImGui::End();
}
#include "Slow.h"
#include "imgui.h"
#include "CsvLoader.h"
#include "Audio.h"
Slow* Slow::GetInstance()
{
	static Slow instance;

	return &instance;
}
//CSV
void Slow::LoadCSV() {
	//m_TargetTimer = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/effect/Slow.csv", "Timer")));
}
//������
void Slow::Initialize() {
	m_Slow = false;//�q�b�g�X�g�b�v
}
//�X�V
void Slow::Update() {
	//���Ԍo�߂�Slow��������ꍇ�p�Ɏc���Ă�
}
void Slow::ImGuiDraw() {
	ImGui::Begin("Slow");
	ImGui::Text("%d", m_Slow);
	ImGui::End();
}
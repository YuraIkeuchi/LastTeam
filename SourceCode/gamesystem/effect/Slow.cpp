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
//初期化
void Slow::Initialize() {
	m_Slow = false;//ヒットストップ
}
//更新
void Slow::Update() {
	//時間経過でSlowが解ける場合用に残してる
}
void Slow::ImGuiDraw() {
	ImGui::Begin("Slow");
	ImGui::Text("%d", m_Slow);
	ImGui::End();
}
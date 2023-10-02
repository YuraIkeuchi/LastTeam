#include "VolumManager.h"
#include "Helper.h"
#include "imgui.h"

VolumManager* VolumManager::GetInstance()
{
	static VolumManager instance;

	return &instance;
}

void VolumManager::Initialize() {

}

void VolumManager::Update() {
	//‰¹—Ê‚Í“Á’è‚Ü‚Å‚µ‚©“®‚©‚È‚¢
	Helper::GetInstance()->Clamp(m_SEVolum, 0.0f, 1.0f);
	Helper::GetInstance()->Clamp(m_BGMVolum, 0.0f, 1.0f);
}

void VolumManager::Draw() {
	/*ImGui::Begin("Volum");
	ImGui::SetWindowPos(ImVec2(1000, 50));
	ImGui::SetWindowSize(ImVec2(280, 300));
	ImGui::SliderFloat("s_BGMVolum", &s_BGMVolum, 360, -360);
	ImGui::SliderFloat("s_SEVolum", &s_SEVolum, 360, -360);
	ImGui::End();*/
}

void VolumManager::Finalize() {

}
#include "SpecialSkill.h"
#include "imgui.h"

void SpecialSkill::ImGui_Origin() {
	ImGui::Begin("Special");
	ImGui::Text("DeckIn:%d", m_DeckIn);
	ImGui::End();
}
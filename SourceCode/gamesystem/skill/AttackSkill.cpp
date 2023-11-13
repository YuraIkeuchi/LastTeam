#include "AttackSkill.h"
#include "imgui.h"

void AttackSkill::ImGui_Origin() {
	ImGui::Begin("Attack");
	ImGui::Text("DeckIn:%d", m_DeckIn);
	ImGui::End();
}
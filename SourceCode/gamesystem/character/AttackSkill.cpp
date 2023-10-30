#include "AttackSkill.h"
#include "imgui.h"

void AttackSkill::ImGui_Origin() {
	ImGui::Begin("Attack");
	ImGui::Text("ID:%d", ID);
	ImGui::Text("DeckIn:%d", m_DeckIn);
	ImGui::End();
}
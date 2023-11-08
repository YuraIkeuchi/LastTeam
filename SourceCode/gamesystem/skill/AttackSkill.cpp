#include "AttackSkill.h"
#include "imgui.h"

void AttackSkill::ImGui_Origin() {
	ImGui::Begin("Attack");
	ImGui::Text("ID:%d", ID);
	ImGui::Text("Damage:%f", damege);
	ImGui::Text("SkillType:%d", skillType);
	ImGui::Text("State:%s", StateName);
	ImGui::End();
}
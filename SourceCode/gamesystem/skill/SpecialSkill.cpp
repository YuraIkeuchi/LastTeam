#include "SpecialSkill.h"
#include "imgui.h"

void SpecialSkill::ImGui_Origin() {
	ImGui::Begin("Special");
	ImGui::Text("ID:%d", ID);
	ImGui::Text("State:%s", StateName);
	ImGui::End();
}
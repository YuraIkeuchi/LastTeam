#include "SkillBuildToul.h"

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

void SkillBuildToul::BuildToul()
{
	ImGui::Begin("SkillBuildToul");

	// 保存ボタン
	{
		if (ImGui::Button("Save"))
		{

		}
	}

	// スキルタイプ用コンボボックス
	{
		// スキルタイプ選択用文字列
		static const char *skill_type_name[] =
		{
			"Attack","Buff","Debuff","Special"
		};
		int temp_skill_type = static_cast<int>(type);
		ImGui::Combo("SkillType", &temp_skill_type, skill_type_name, IM_ARRAYSIZE(skill_type_name));
		type = static_cast<TestType>(temp_skill_type);

	}

	// 待機時間
	{
		ImGui::DragInt("Latency", &latency_);
	}

	ImGui::End();

}
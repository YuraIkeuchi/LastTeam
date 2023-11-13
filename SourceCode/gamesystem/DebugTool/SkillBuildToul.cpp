#include "SkillBuildToul.h"

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

void SkillBuildToul::BuildToul()
{
	ImGui::Begin("SkillBuildToul");

	// �ۑ��{�^��
	{
		if (ImGui::Button("Save"))
		{

		}
	}

	// �X�L���^�C�v�p�R���{�{�b�N�X
	{
		// �X�L���^�C�v�I��p������
		static const char *skill_type_name[] =
		{
			"Attack","Buff","Debuff","Special"
		};
		int temp_skill_type = static_cast<int>(type);
		ImGui::Combo("SkillType", &temp_skill_type, skill_type_name, IM_ARRAYSIZE(skill_type_name));
		type = static_cast<TestType>(temp_skill_type);

	}

	// �ҋ@����
	{
		ImGui::DragInt("Latency", &latency_);
	}

	ImGui::End();

}
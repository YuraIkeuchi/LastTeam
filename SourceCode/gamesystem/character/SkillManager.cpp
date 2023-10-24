#include "SkillManager.h"

SkillManager* SkillManager::GetInstance()
{
	static SkillManager instance;

	return &instance;
}

void SkillManager::Initialize()
{
	//一旦3に指定(実際はCSVとかになるかな)
	skill.resize(3);
	//ここはもう少しやりようがあるかもしれない
	skill[0] = new NormalSkill();
	skill[0]->Create(nameA, 5);
	skill[1] = new NormalSkill();
	skill[1]->Create(nameB, 2, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[2] = new AttackSkill();
	skill[2]->Create(nameC, 8, 0.0f, 0.0f, 0.0f, 1, 1);
}

void SkillManager::ImGuiDraw() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->ImGuiDraw();
		}
	}
}
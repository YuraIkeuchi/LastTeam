#include "SkillManager.h"
#include <random>
#include <Helper.h>
SkillManager* SkillManager::GetInstance()
{
	static SkillManager instance;

	return &instance;
}

void SkillManager::Initialize()
{
	//一旦3に指定(実際はCSVとかになるかな)
	skill.resize(5);
	//ここはもう少しやりようがあるかもしれない
	skill[0] = new NormalSkill();
	skill[0]->Create(nameA, 1);
	skill[1] = new NormalSkill();
	skill[1]->Create(nameB, 2, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[2] = new NormalSkill();
	skill[2]->Create(nameC, 3, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[3] = new NormalSkill();
	skill[3]->Create(nameD, 4, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[4] = new AttackSkill();
	skill[4]->Create(nameE, 5, 0.0f, 0.0f, 0.0f, 1, 1);

	//順番入れ替えてる
	std::shuffle(skill.begin(), skill.end(), std::default_random_engine());
}

void SkillManager::ImGuiDraw() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->ImGuiDraw();
		}
	}
}

int SkillManager::GetID() {
	int result;
	int randskill;
	//ランダムのスキルのIDをもらう
	randskill = Helper::GetInstance()->GetRanNum(0, (int)(skill.size() - 1));
	result = skill[randskill]->GetID();

	return result;
}
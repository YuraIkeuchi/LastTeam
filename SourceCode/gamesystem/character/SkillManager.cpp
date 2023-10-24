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
	//��U3�Ɏw��(���ۂ�CSV�Ƃ��ɂȂ邩��)
	skill.resize(6);
	//�����͂����������悤�����邩������Ȃ�
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
	skill[5] = new AttackSkill();
	skill[5]->Create(nameF, 6, 0.0f, 0.0f, 0.0f, 1, 1);

	//���ԓ���ւ��Ă�
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
	int result = 0;
	int randskill = 0;

	//�����_���Ŏ擾(�����͌�Œ���)
	randskill = Helper::GetInstance()->GetRanNum(0, (int)(skill.size() - 1));
	if (!skill[randskill]->GetBirth()) {
		skill[randskill]->SetBirth(true);
		result = skill[randskill]->GetID();
	}
	else {
		randskill = Helper::GetInstance()->GetRanNum(0, (int)(skill.size() - 1));
		skill[randskill]->SetBirth(true);
		result = skill[randskill]->GetID();
	}
	return result;
}

void SkillManager::ResetBirth() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->SetBirth(false);
		}
	}
}
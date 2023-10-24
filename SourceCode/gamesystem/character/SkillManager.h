#pragma once
#include "SkillBase.h"
#include "AttackSkill.h"
#include "NormalSkill.h"
#include "AttackSkill.h"
#include <string>
#include <vector>

class SkillManager
{
public:
	static SkillManager* GetInstance();
	
	void Initialize();

	void ImGuiDraw();
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "����";
	std::string nameB = "�R��";
	std::string nameC = "�@��";
};
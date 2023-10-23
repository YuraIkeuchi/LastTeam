#pragma once
#include "SkillBase.h"
#include "AttackSkill.h"
#include <string>

class SkillManager
{
public:
	static SkillManager* GetInstance();
	
	void Initialize();


private:
	SkillBase skillA;
	SkillBase skillB;
	AttackSkill skillC;
	std::string nameA = "â£ÇÈ";
	std::string nameB = "èRÇÈ";
	std::string nameC = "í@Ç≠";
};
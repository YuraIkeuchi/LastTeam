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

	void ResetBirth();
public:
	//IDÇéÊÇÈ
	int GetID();
public:
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "â£ÇÈ";
	std::string nameB = "èRÇÈ";
	std::string nameC = "í@Ç≠";
	std::string nameD = "éEÇÈ";
	std::string nameE = "Ç©Ç‹Ç∑";
	std::string nameF = "ÉXÉJÉX";
};
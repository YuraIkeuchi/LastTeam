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

public:
	//IDðæé
	int GetID();
public:
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "£é";
	std::string nameB = "Ré";
	std::string nameC = "@­";
	std::string nameD = "Eé";
	std::string nameE = "©Ü·";
};
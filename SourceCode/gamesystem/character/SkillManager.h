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

private:
	//ƒXƒLƒ‹‚ÌCSV‚ğ“Ç‚İæ‚é
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSV‚ğì¬
	bool CreateSkill(int id);
public:
	//ID‚ğæ‚é
	int GetID();
	float GetDamage();
public:
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "‰£‚é";
	std::string nameB = "R‚é";
	std::string nameC = "’@‚­";
	std::string nameD = "E‚é";
	std::string nameE = "‚©‚Ü‚·";
	std::string nameF = "ƒXƒJƒX";
	int m_RandNum = {};
};
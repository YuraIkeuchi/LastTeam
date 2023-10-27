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
	//�X�L����CSV��ǂݎ��
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSV���쐬
	bool CreateSkill(int id);
public:
	//ID�����
	int GetID();
public:
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "����";
	std::string nameB = "�R��";
	std::string nameC = "�@��";
	std::string nameD = "�E��";
	std::string nameE = "���܂�";
	std::string nameF = "�X�J�X";
};
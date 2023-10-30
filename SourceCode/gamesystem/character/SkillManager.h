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
	//デッキに入ってるかチェック
	void DeckCheck(const int DeckNumber);
	//デッキのクリア
	void DeckClear();
private:
	//スキルのCSVを読み取る
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVを作成
	bool CreateSkill(int id);
public:
	//IDを取る
	int GetID(const int BirthNum);
	//ダメージを取る
	float GetDamage();
	//ディレイを取る
	int GetDelay();
	void SetDeckState(const int DeckNum);
public:
	std::vector<SkillBase*>GetSkillBase() { return skill; }
	const int GetDeckNum() { return m_DeckNum; }
	
private:
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "殴る";
	std::string nameB = "蹴る";
	std::string nameC = "叩く";
	std::string nameD = "殺る";
	std::string nameE = "かます";
	std::string nameF = "スカス";
	int m_DeckNum = {};
	int m_BirthMax = {};
	int m_DeckRemain = {};
	std::vector<int> m_DeckDate;
};
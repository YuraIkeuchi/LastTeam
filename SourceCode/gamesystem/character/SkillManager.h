#pragma once
#include "SkillBase.h"
#include "AttackSkill.h"
#include "NormalSkill.h"
#include "AttackSkill.h"
#include <string>
#include <vector>
#include "DeckUI.h"
class SkillManager
{
public:
	static SkillManager* GetInstance();

	void Initialize();

	void Update();

	void UIDraw();
	void ImGuiDraw();

	void ResetBirth();
	//デッキに入ってるかチェック
	void DeckCheck(const int DeckNumber,const int DeckCount);
	//デッキのクリア
	void DeckClear();
	void GetAreaDate(int& DisX, int& DisY);
private:
	//スキルのCSVを読み取る
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVを作成
	bool CreateSkill(int id);
	//UIの生成
	void BirthDeckUI(const int DeckNumber, const int DeckCount);
public:
	//IDを取る
	int GetID(const int BirthNum);
	//ダメージを取る
	float GetDamage();
	//ディレイを取る
	int GetDelay();

	std::vector<std::vector<int>> GetArea();
	void SetDeckState(const int DeckNum);
public:
	const int GetDeckNum() { return m_DeckNum; }
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<unique_ptr<DeckUI>> deckui;
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
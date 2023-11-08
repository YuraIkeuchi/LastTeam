#pragma once
#include "SkillBase.h"
#include "AttackSkill.h"
#include "NormalSkill.h"
#include "SpecialSkill.h"
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
	void GetSkillType(int& SkillType);
	void GetAttackSkillData(float& damage, int& delay, vector<std::vector<int>>& area, int& DisX, int& DisY,string& name);
	void GetSpecialSkillDate(int& delay,string& name);

	void SetDeckState(const int DeckNum);
public:
	const int GetDeckNum() { return m_DeckNum; }
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<unique_ptr<DeckUI>> deckui;
	std::vector<SkillBase*> skill;
	int m_DeckNum = {};
	int m_BirthMax = {};
	int m_DeckRemain = {};
	std::vector<int> m_DeckDate;
};
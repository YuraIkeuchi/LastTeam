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
	
	void PushOnce2Deck(const int DeckNumber);
	//リザルト画面に渡すためのデータ
	void HandResultData(const int DeckID,vector<std::vector<int>>& area, int& DisX, int& DisY,int& Damage);
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
	//IDを検索
	int IDSearch(const int BirthNum);
	void GetSkillType(int& SkillType);
	void GetAttackSkillData(float& damage, int& delay, vector<std::vector<int>>& area, vector<std::vector<int>>& timer, int& DisX, int& DisY,string& name,
		int& Token);
	void GetSpecialSkillDate(int& delay,string& name);

	void SetDeckState(const int DeckNum);
public:
	const int GetDeckUISize() { return (int)deckui.size(); }
	const int GetDeckNum() { return m_DeckNum; }
	const int GetSkillMax() { return m_SKILLMAX; }
	const int GetDelay(int id) { return m_Delays[id]; }
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<unique_ptr<DeckUI>> deckui;
	std::vector<SkillBase*> skill;
	std::vector<int> m_Delays = {};
	int m_DeckNum = {};
	int m_BirthNow = {};
	int m_DeckRemain = {};
	int m_SKILLMAX = 0;
	std::vector<int> m_DeckDate;
};
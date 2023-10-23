#pragma once
#include "IKESprite.h"
#include <string>
#include <vector>
#include <memory>
#include "ActionUI.h"
#include "AttackArea.h"
using namespace DirectX;
using namespace std;
//行動の種類
enum ActType {
	ACT_ATTACK,
	ACT_GUARD,
	ACT_SKILL
};
//ゲームの状態を管理するクラス
class GameStateManager {
public:
	static GameStateManager* GetInstance();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
	//行動UIの描画(あんまり良くないが一旦ここで)
	void ActUIDraw();
	//プレイヤーの現在位置
	void PlayerNowPanel(const int NowWidth, const int NowHeight);
	//スキルを入手する
	void AddSkill(const string& Tag);
private:
	
	//攻撃エリアの生成
	void BirthArea();
	//行動UIの生成
	void BirthActUI(const string& Tag);
	//スキルの使用
	void UseSkill();
	//行動の終了
	void FinishAct();

public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }
	vector<AttackArea*>GetAttackArea() { return attackarea; }

	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
	
private:
	static const int ACT_PATTERN = 3;
private:
	//各行動回数
	int m_ActCount[ACT_PATTERN] = {};

	//全行動回数
	int m_AllActCount = {};

	//行動のUI
	vector<unique_ptr<ActionUI>> actui;

	//攻撃エリア
	vector<AttackArea*> attackarea;
	//行動先
	vector<int> m_Act;

	//カウンター
	bool m_Counter = false;
	int m_CounterTimer = {};
	int m_CounterScore = {};

	//位置のスコア
	float m_PosScore = {};
	//グレイズのスコア(後々intにする)
	float m_GrazeScore = 0.0f;

	//全体スコア
	int m_AllScore = {};

	//プレイヤーの現在パネル
	int m_NowHeight = {};
	int m_NowWidth = {};

	enum SkillType {
		SKILL_NORMAL,
		SKILL_STRONG,
		SKILL_SPECIAL
	}_SkillType = SKILL_NORMAL;

};
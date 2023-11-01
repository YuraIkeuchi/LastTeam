#pragma once
#include "IKESprite.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "ActionUI.h"
#include "AttackArea.h"
#include <Passive.h>
#include "../../../ResultSkill.h"
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
	void AddSkill(const int ID,const float damage,const int Delay);
private:
	//攻撃した瞬間
	void AttackTrigger();
	//攻撃エリアの生成
	void BirthArea();
	//行動UIの生成
	void BirthActUI(const int ID);
	//スキルの使用
	void UseSkill();
	//行動の終了
	void FinishAct();
	//
	void GaugeUpdate();

	void PassiveCheck();
	//デッキの初期化
	void DeckInitialize();
	void GetPassive(int ID);


	void InputDebug();
	void InDeck();//デッキに組み込む
public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }
	vector<AttackArea*>GetAttackArea() { return attackarea; }
	const float GetDiameterVel() { return m_DiameterVel; }


	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
	void SetDiameterVel(const float DiameterVel) { this->m_DiameterVel = DiameterVel; }
private:
	static const int ACT_PATTERN = 3;
private:

	struct ActState {
		int ActID;//ID
		float ActDamage;//ダメージ
		int ActDelay;//ディレイ
	};

	vector<ActState> m_Act;
	//各行動回数
	int m_ActCount[ACT_PATTERN] = {};

	//全行動回数
	int m_AllActCount = {};

	//行動のUI
	vector<unique_ptr<ActionUI>> actui;

	std::list<std::unique_ptr<Passive>> GotPassives;
	std::vector<int> GotPassiveIDs;
	std::vector<int> NotPassiveIDs;

	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;

	XMFLOAT2 basesize = { 45.f,400.f };

	//攻撃エリア
	vector<AttackArea*> attackarea;

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
	float m_DiameterVel = 1.0f;


	enum SkillType {
		SKILL_NORMAL,
		SKILL_STRONG,
		SKILL_SPECIAL
	}_SkillType = SKILL_STRONG;
	//ゲージ
	float m_GaugeCount = 0;
	//
	float m_DiameterGauge = 1.0f;
	//ゲージマックス
	float kGaugeCountMax = 180;
	bool m_IsReload = true;
	bool m_BirthSkill = false;

	int m_ID = {};
	int m_Delay = {};
	string m_Name;

	vector<int> m_DeckNumber = { 0,2,3,5,6, };
	vector<int> m_NotDeckNumber = {};

	int m_DistanceX = 5;
	int m_DistanceY = 10;
	std::vector<std::vector<int>> m_Area;
	int m_NotCount = {};
	std::unique_ptr<ResultSkill> resultSkill;
};
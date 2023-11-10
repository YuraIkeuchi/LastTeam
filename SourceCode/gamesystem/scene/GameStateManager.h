#pragma once
#include "IKESprite.h"
#include "IKETexture.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "ActionUI.h"
#include "AttackArea.h"
#include "PredictArea.h"
#include <Passive.h>
#include "GameObject/GameObject.h"
#include "Player.h"
#include "BaseEnemy.h"
#include "ResultSkill.h"
using namespace DirectX;
using namespace std;



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
	void AddSkill(const int SkillType, const int ID, const float damage, const int Delay, vector<std::vector<int>> area, int DisX, int DisY, string name);
private:
	void PredictManager();
	//攻撃した瞬間
	void AttackTrigger();
	//攻撃エリアの生成
	void BirthArea();
	//バフ状況
	void BirthBuff();
	//行動UIの生成
	void BirthActUI(const int ID, const int Type);
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
	
	bool AttackSubAction();

	bool SkillRecycle();

	bool ResultUpdate();
	void InDeck();//デッキに組み込む

	bool AttackSubAction();
public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const bool GetIsChangeScene() { return isChangeScene; }
	const bool GetIsFinish() { return isFinish; }
	const bool GetBuff() { return m_Buff; }

	const bool GetPoisonSkill() { return  m_poizonLong; }
	const bool GetPoisonVenom() { return  m_IsVenom; }

	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }
	vector<AttackArea*>GetAttackArea() { return attackarea; }
	const float GetDiameterVel() { return m_DiameterVel; }
	std::weak_ptr<Player> GetPlayer() { return player_; }

	/// <summary>
	/// 敵を倒したら最初の処理
	/// </summary>
	void StageClearInit();
	void SetEnemySpawnText(string& text) { enemySpawnText = text; }
	string& GetEnemySpawnText() { return enemySpawnText; }
	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetResetPredict(const bool ResetPredict) { this->m_ResetPredict = ResetPredict; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
	void SetDiameterVel(const float DiameterVel) { this->m_DiameterVel = DiameterVel; }
	void SetPlayer(std::weak_ptr<Player> player) { player_ = player; }
	// 仮
	void SetBuff(const bool Buff) { this->m_Buff = Buff; }
private:
	unique_ptr<IKETexture> _charge;
	bool isFinish = false;
	bool isChangeScene = false;
	struct ActState {
		int SkillType;//スキルの種類
		int ActID;//ID
		float ActDamage;//ダメージ
		int ActDelay;//ディレイ
		vector<std::vector<int>> AttackArea;	//範囲
		int DistanceX;		//プレイヤーからの距離
		int DistanceY;		//プレイヤーからの距離
		string StateName;		//付与状態
	};

	vector<ActState> m_Act;

	//全行動回数
	int m_AllActCount = {};

	//行動のUI
	vector<unique_ptr<ActionUI>> actui;

	std::list<std::unique_ptr<Passive>> GotPassives;
	std::vector<int> GotPassiveIDs = {5};
	std::vector<int> NotPassiveIDs;

	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;

	XMFLOAT2 basesize = { 45.f,400.f };

	// 攻撃エリア
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

	string enemySpawnText = "Resources/csv/EnemySpawn/BattleMap01.csv";

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

	bool m_poizonLong = false;
	bool m_IsVenom = false;
	bool m_IsRecycle = false;

	bool m_BirthSkill = false;

	int m_ID = {};
	int m_Delay = {};
	string m_Name;

	vector<int> m_DeckNumber = { 6,7,8,9 };


	vector<int> m_NotDeckNumber = {};

	int m_DistanceX = 0;
	int m_DistanceY = 0;
	int m_NotCount = {};

	//予測エリア
	unique_ptr<PredictArea> predictarea;

	bool m_ResetPredict = false;
	std::unique_ptr<ResultSkill> resultSkill;

	//ディレイ関係
	float m_ChargeScale = {};
	int m_DelayTimer = {};
	bool m_DelayStart = false;

	//バフ(一旦一個)
	bool m_Buff = false;


	///=============================
	/// 
	/// プレイヤー関連
	/// 
	/// =============================

	// Playerポインタ
	std::weak_ptr<Player> player_;


	/// =============================
	/// 
	/// エネミー関連
	/// 
	/// =============================
	
	/// <summary>
	/// エネミー管理用のコンテナ
	/// </summary>
	vector<weak_ptr<BaseEnemy>> enemys_container_;
};
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
#include "HaveResultSkill.h"
#include <Onomatope.h>
#include "ResultReport.h"

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
	void AddSkill(const int SkillType, const int ID, const float damage, const int Delay,
		vector<std::vector<int>> area, vector<std::vector<int>> timer, int DisX, int DisY, string name);

	/// <summary>
	/// エネミーのデータをコンテナに追加
	/// </summary>
	/// <param name="Enemy"></param>
	/// <param name="作者">やぶなか</param>
	void AddEnemy(std::weak_ptr<BaseEnemy> Enemy) { enemys_container_.emplace_back(Enemy); }
	//デッキのリセット
	void DeckReset();
	//捨てたIDを取得する
	void GetDiscardSkill(const int ID);
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
	void DeckDiscard();
	void GetPassive(int ID);
	
	bool AttackSubAction();

	bool SkillRecycle();

	bool ResultUpdate();
	void InDeck();//デッキに組み込む
	void RandPowerUpInit();
	void PowerUpEffectUpdate();

	void PassiveActive();
public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const bool GetIsChangeScene() { return isChangeScene; }
	const bool GetIsFinish() { return isFinish; }
	const bool GetBuff() { return m_Buff; }

	const bool GetPoisonSkill() { return  m_poizonLong; }
	const bool GetPoisonVenom() { return  m_IsVenom; }
	const bool GetResetPredict() { return  m_ResetPredict; }

	void SetDxCommon(DirectXCommon* dxCommon) { this->m_dxCommon = dxCommon; }
	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }
	vector<unique_ptr<AttackArea>>& GetAttackArea() { return attackarea; }
	const float GetDiameterVel() { return m_DiameterVel; }
	//std::weak_ptr<Player> GetPlayer() { return player_; }

	/// <summary>
	/// 敵を倒したら最初の処理
	/// </summary>
	void StageClearInit();
	void SetEnemySpawnText(string& text, bool isBattle=true) { enemySpawnText = text; isBattleFromMap = isBattle; }
	string& GetEnemySpawnText() { return enemySpawnText; }
	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetResetPredict(const bool ResetPredict) { this->m_ResetPredict = ResetPredict; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
	void SetDiameterVel(const float DiameterVel) { this->m_DiameterVel = DiameterVel; }
	//void SetPlayer(std::weak_ptr<Player> player) { player_ = player; }
	// 仮
	void SetBuff(const bool Buff) { this->m_Buff = Buff; }

	void SetIsReloadDamage(bool flag) { m_ReloadDamage = flag; }
	bool GetIsReloadDamage() { return m_ReloadDamage; }

	void SetIsBombDamage(bool flag) { m_BombDamage = flag; }
	bool GetIsBombDamage() { return m_BombDamage; }

	void SetIsHeal(bool flag) { m_Heal = flag; }
	bool GetIsHeal() { return m_Heal; }

	void SetGameStart(bool GameStart) { m_GameStart = GameStart;}
	bool GetGameStart() { return m_GameStart; }

	void DamageCheck(int Damage);
	void TakenDamageCheck(int Damage);
	bool GetIsFivePower() { return m_FivePower && (m_HandedCount % 5 == 0); }

	void SetPassiveActive();
	
public:
	static void SetPlayer(Player* player) { GameStateManager::player = player; }
private:

	static Player* player;
	DirectXCommon* m_dxCommon = nullptr;
	unique_ptr<IKETexture> _charge;
	bool isFinish = false;
	bool isChangeScene = false;
	bool isResultFinish = false;
	struct ActState {
		int SkillType;//スキルの種類
		int ActID;//ID
		float ActDamage;//ダメージ
		int ActDelay;//ディレイ
		vector<std::vector<int>> AttackArea;	//範囲
		vector<std::vector<int>> AttackTimer;	//範囲
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
	std::vector <int> m_StartPassive= {7};
	std::vector<int> GotPassiveIDs = m_StartPassive;
	std::vector<int> NotPassiveIDs;

	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;
	unique_ptr<IKESprite> gaugeCover = nullptr;
	unique_ptr<IKESprite> passiveActive = nullptr;
	bool isPassive = false;
	float passiveFrame = 0.f;
	float passiveAlpha = 1.0f;
	float passiveAlphaFrame = 0.0f;

	std::unique_ptr<IKESprite> handsFrame;

	XMFLOAT2 basesize = { 46.f,400.f };

	// 攻撃エリア
	vector<unique_ptr<AttackArea>> attackarea;

	unique_ptr<Onomatope> onomatope = nullptr;
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
	int m_MaxDamage = 0;
	int m_MaxTakenDamage = 0;
	int m_HandedCount = 0;

	string enemySpawnText = "Resources/csv/EnemySpawn/BattleMap01.csv";
	bool isBattleFromMap = true;
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
	float kGaugeCountMax = 150;
	bool m_IsReload = true;
	bool m_IsReloadDamage = false;
	bool m_ReloadDamage = false;
	bool m_BombDamage = false;
	bool m_Heal = false;
	bool m_poizonLong = false;
	bool m_IsVenom = false;
	bool m_IsDrainUp = false;
	bool m_FivePower = false;

	bool m_BirthSkill = false;

	int m_ID = {};
	int m_Delay = {};
	string m_Name;

	vector <int> m_StartNumber = { 1,2,9,10 };
	vector<int> m_DeckNumber = m_StartNumber;

	vector<int> m_NotDeckNumber = {};
	vector<int> m_DiscardNumber = {};

	int m_DistanceX = 0;
	int m_DistanceY = 0;
	int m_NotCount = {};

	//予測エリア
	unique_ptr<PredictArea> predictarea;

	bool m_ResetPredict = false;
	int m_PredictTimer = {};
	std::unique_ptr <ResultReport> resultReport;
	std::unique_ptr<ResultSkill> resultSkill;
	std::unique_ptr<HaveResultSkill> haveSkill;
	//ディレイ関係
	float m_ChargeScale = {};
	int m_DelayTimer = {};
	bool m_DelayStart = false;

	//バフ(一旦一個)
	bool m_Buff = false;
	bool m_Choice = false;
	enum ResultType {
		GET_SKILL,
		HAVE_SKILL,
	}_ResultType = GET_SKILL;

	struct PowerUpEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float kFrame = 30.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 afterpos = {};
		XMFLOAT2 size = { 48.f,48.f };
		XMFLOAT4 color = { 1.5f,1.5f,1.5f,1.0f };
		bool isVanish = false;
	};
	std::list<PowerUpEffect> powerup;

	bool m_GameStart = false;

	///=============================
	/// 
	/// プレイヤー関連
	/// 
	/// =============================

	/// <summary>
	/// プレイヤーのポインタ
	/// </summary>
	
	//プレイヤーの現在パネル
	int m_NowHeight = {};		// Yパネル座標
	int m_NowWidth = {};		// Xパネル座標

	// ゲージ速度
	float m_DiameterVel = 1.0f;

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
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
#include "RegeneArea.h"
#include <Passive.h>
#include "GameObject/GameObject.h"
#include "Player.h"
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
		vector<std::vector<int>> area, vector<std::vector<int>> timer, int DisX, int DisY, string name, int Token);

	//デッキのリセット
	void DeckReset();
	//捨てたIDを取得する
	void GetDiscardSkill(const int ID);
	//攻撃失敗
	void MissAttack();
	//
	void DamageEffectInit(XMFLOAT2 pos);

	void SaveGame();

	void OpenGameDate();

	void DeleteDeck(const int num);
private:
	void PredictManager();
	//攻撃した瞬間
	void AttackTrigger();
	//攻撃エリアの生成
	void BirthArea();
	bool GetIsFix(const string& name);
	//バフ状況
	void BirthBuff(string& stateName);
	//行動UIの生成
	void BirthActUI(const int ID, const int Type);
	//スキルの使用
	void UseSkill();
	//行動の終了
	void FinishAct(bool AllFinish = false);
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

	void DamageEffectUpdate();
public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const bool GetIsChangeScene() { return isChangeScene; }
	const bool GetIsFinish() { return isFinish; }

	const bool GetPoisonSkill() { return  m_poizonLong; }
	const bool GetPoisonVenom() { return  m_IsVenom; }
	const bool GetResetPredict() { return  m_ResetPredict; }

	void SetDxCommon(DirectXCommon* dxCommon) { this->m_dxCommon = dxCommon; }
	const int GetPosScore() { return m_PosScore; }
	vector<unique_ptr<AttackArea>>& GetAttackArea() { return attackarea; }
	const float GetDiameterVel() { return m_DiameterVel; }
	//std::weak_ptr<Player> GetPlayer() { return player_; }

	/// <summary>
	/// 敵を倒したら最初の処理
	/// </summary>
	void StageClearInit();
	void SetEnemySpawnText(string& text, bool isBattle = true) { enemySpawnText = text; isBattleFromMap = isBattle; }
	string& GetEnemySpawnText() { return enemySpawnText; }
	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetResetPredict(const bool ResetPredict) { this->m_ResetPredict = ResetPredict; }
	void SetPosScore(const int PosScore) { this->m_PosScore = PosScore; }
	void SetDiameterVel(const float DiameterVel) { this->m_DiameterVel = DiameterVel; }
	//void SetPlayer(std::weak_ptr<Player> player) { player_ = player; }
	// 仮
	void SetIsReloadDamage(bool flag) { m_ReloadDamage = flag; }
	bool GetIsReloadDamage() { return m_ReloadDamage; }

	void SetIsBombDamage(bool flag) { m_BombDamage = flag; }
	bool GetIsBombDamage() { return m_BombDamage; }

	void SetIsHeal(bool flag) { m_Heal = flag; }
	bool GetIsHeal() { return m_Heal; }

	void SetIsBomSuccess(bool flag) { isBomSuccess = flag; }
	bool GetIsBomSuccess() { return isBomSuccess; }

	void SetCounterBuff(bool flag) { m_CounterBuff = flag; }
	bool GetCounterBuff() { return m_CounterBuff; }

	void SetGameStart(bool GameStart) { m_GameStart = GameStart; }
	bool GetGameStart() { return m_GameStart; }

	void SetNoDeath(bool flag) { m_NoDeath = flag; }
	bool GetNoDeath() { return m_NoDeath; }

	void SetNoDamage(bool flag) { m_NoDamage = flag; }
	bool GetNoDamage() { return m_NoDamage; }

	void SetBossCamera(bool BossCamera) { m_BossCamera = BossCamera; }
	bool GetBossCamera() { return m_BossCamera; }

	void DamageCheck(int Damage);
	void TakenDamageCheck(int Damage);
	bool GetIsFivePower() { return m_FivePower && (m_HandedCount % 5 == 0); }
	bool GetTakenDamageUp() { return m_TakenDamageUp; }
	bool GetAttackedPoison() { return m_AttackedPoison; }
	bool GetHealDamage() { return m_healingDamage; }
	void SetHealDamage(bool flag) { m_healingDamage = flag; }

	bool GetExtendKnight() { return m_ExtendKnight && (player->GetNowWidth() == 3); }
	bool GetExtendRook() { return m_ExtendRook; }
	bool GetExtendQueen() { return m_ExtendQueen; }
	bool GetExtendBishop() { return m_ExtendBishop; }

	int GetRookPoison() { return m_RookPoison; }
	void AddRookPoison(int add) { m_RookPoison += add; }
	void SetRookPoison(int num) { m_RookPoison = num; }

	void SetPassiveActive(int id);

	int GetTakenDamageNum() { return m_TakenDamageNum; }
	void SetTakenDamageNum(int num) { m_TakenDamageNum = num; }

	void SetMapData(int index, int Hierarchy) { savedata.m_SaveIndex = index; savedata.m_SaveHierarchy = Hierarchy; }
	void SetSaveHP(float hp) { savedata.m_SaveHP = hp; }

	//セーブデータ
	const float GetOpenHP() { return savedata.m_OpenHP; }
	const int GetHierarchy() { return savedata.m_OpenHierarchy; }
	const int GetIndex() { return savedata.m_OpenIndex; }

	bool GetEndResult() { return m_EndResult; }
	void SetEndText(const bool EndText) { m_EndText = EndText; }
public:
	static void SetPlayer(Player* player) { GameStateManager::player = player; }
private:

	static Player* player;
	DirectXCommon* m_dxCommon = nullptr;
	unique_ptr<IKETexture> _charge;
	bool isFinish = false;
	bool isChangeScene = false;
	bool isResultFinish = false;
	bool isBomSuccess = false;
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
		int PoisonToken;		//ポイズンの時間
	};

	struct DamageEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float frameA = 0.f;
		float kFrame = 20.f;
		float angle = 0.f;
		float dia = 0.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 32.f,32.f };
		bool isVanish = false;
	};

	vector<ActState> m_Act;

	//全行動回数
	int m_AllActCount = {};

	//行動のUI
	vector<unique_ptr<ActionUI>> actui;

	std::list<DamageEffect> damages;

	std::list<std::unique_ptr<Passive>> GotPassives;
	std::vector <int> m_StartPassive= {};
	std::vector<int> GotPassiveIDs = m_StartPassive;
	std::vector<int> NotPassiveIDs;

	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;
	unique_ptr<IKESprite> gaugeCover = nullptr;
	unique_ptr<IKESprite> passiveActive = nullptr;
	std::vector<unique_ptr<IKESprite>> passiveActs;
	std::vector<int> passiveActiveNum;

	bool isPassive = false;
	float passiveFrame = 0.f;
	float passiveAlpha = 1.0f;
	float passiveAlphaFrame = 0.0f;

	std::unique_ptr<IKESprite> handsFrame;
	unique_ptr<IKESprite> cancelSkill;

	XMFLOAT2 basesize = { 46.f,350.f };

	// 攻撃エリア
	vector<unique_ptr<AttackArea>> attackarea;
	// 回復エリア
	vector<unique_ptr<RegeneArea>> regenearea;
	unique_ptr<Onomatope> onomatope = nullptr;
	//カウンター
	bool m_Counter = false;
	int m_CounterTimer = {};
	int m_CounterCount = {};
	bool m_CounterBuff = false;
	//位置のスコア
	int m_PosScore = {};

	//全体スコア
	int m_AllScore = {};
	int m_OldDamage = 0;
	int m_MaxDamage = 0;
	int m_MaxTakenDamage = 0;
	int m_TakenDamageNum = 0;
	int m_HandedCount = 0;

	int m_Metronome = 0;
	float m_MetroDamage = 8.f;
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
	bool m_FivePower = false;
	bool m_TakenDamageUp = false;
	bool m_AttackedPoison = false;
	bool m_healingDamage = false;
	bool m_ExtendKnight = false;
	bool m_ExtendRook = false;
	bool m_ExtendQueen = false;
	bool m_ExtendBishop = false;

	int m_RookPoison = 0;

	bool m_BirthSkill = false;

	int m_ID = {};
	int m_Delay = {};
	string m_Name;
	vector <int> m_StartNumber = {2,3,5,6};

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
	bool m_Shield = false;
	int m_ShieldCount = {};
	int m_DeleteNum = 0;

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
	bool m_BossCamera = false;
	bool m_StartLoad = false;

	bool m_NoDeath = true;
	bool m_NoDamage = true;
	bool m_EndResult = false;
	bool m_EndText = false;
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
	struct SaveData {
		int m_SaveIndex = {};
		int m_SaveHierarchy = {};
		float m_SaveHP = {};
		int m_OpenIndex = {};
		int m_OpenHierarchy = {};
		float m_OpenHP = {};
		vector<int> m_OpenDeckNumber = {};
		vector<int> m_OpenPassiveNumber = {};
		int m_DeckNum = {};
		int m_PassiveNum = {};
		//普通の敵
		std::ifstream dataFile;
		std::stringstream dataPopcom;
		std::string dataLine;
	};

	SaveData savedata;
};
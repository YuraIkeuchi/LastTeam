﻿#pragma once
#include "ObjCommon.h"
#include <IKESprite.h>
#include <IKETexture.h>
#include <memory>
#include <Input.h>
#include <array>
#include "DrawNumber.h"
#include "DrawHealNumber.h"

using namespace DirectX;

class Player :
	public ObjCommon
{
private:
	static void (Player::* stateTable[])();
public:

	void InitState(const XMFLOAT3& pos);
	void LoadResource();
	void SkipInitialize();
	//初期化
	bool Initialize() override;
	void AwakeUpdate();
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon *dxCommon) override;
	//UI用
	void UIDraw();
	//ImGui
	void ImGuiDraw();
	//チュートリアルの更新
	void TitleUpdate();

	void SetTitleFlag(bool flag) { is_title = flag; }
	//ゲームクリアの動き
	void ClearUpdate();
	//ゲームオーバーの動き
	void DeathUpdate();
	//ゲームオーバーの動き
	void GameOverUpdate(const int Timer);
private:
	//動き
	void Move();

	bool MoveButtonKey();
	bool UpButtonKey();
	bool DownButtonKey();
	bool RightButtonKey();
	bool LeftButtonKey();


	//動きが止まる
	void Delay();

	//パーティクル
	void BirthParticle();
	//HPの割合を求める
	float OldHpPercent();

public:
	//HPの割合を求める
	float HpPercent();
	//回復量のセット
	void SetHealPower(const float power);
	//ドレイン回復
	void DrainHeal();
	//プレイヤーの回復
	void RegeneHeal(const float power);

	//プレイヤーのダメージ
	void RecvDamage(const float Damage,const string& name = "NORMAL");
	//プレイヤーの情報保存
	void PlayerSave();
private:
	//回復の基本
	void HealCommon(const float power);
	void MoveCommon(float& pos, const float velocity);
private:
	void LoadCSV();
	//パーティクル(回復)
	void HealParticle();
	//ダメージのパーティクル
	void DamageParticle();
	//毒のパーティクル
	void BirthPoisonParticle();
	//ダメージの更新
	void DamageUpdate();
	void BirthHealNumber(const float heal);
public:
	//getter setter
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
	const int GetCharaState() { return _charaState; }
	const bool GetDelay() { return m_Delay; }
	const bool GetCancel() { return m_Cancel; }

	const bool GetFinishGameOver() { return m_FinishGameOver; }
	const bool GetSelectEnd() { return m_SelectEnd; }
	const bool GetMove() { return m_Move; }
	const bool GetSelectMove() { return m_SelectMove; }
	const int GetSelectType() { return m_SelectType; }
	const int GetOverType() { return (int)_OverType; }
	float GetMaxHp() { return m_MaxHP; }
	float GetHp() { return m_HP; }

	void SetHp(float hp) { m_HP = hp; }
	void ResetHp() {
		m_HP = 500.f;
		m_MaxHP = 500.f;
		isHpPassive = false;
	}


	void SetHealingDamage(bool flag) { m_healingDamage = flag; }

	void SetGrazePos(const XMFLOAT3& GrazePos) { m_GrazePos = GrazePos; }

	void SetDelay(const bool Delay) { m_Delay = Delay; }
	void SetSelectEnd(const bool SelectEnd) { m_SelectEnd = SelectEnd; }

	void SetCancel(const bool cancel) { m_Cancel = cancel; }
	void SetDrawShield(const bool DrawShield) { m_DrawShield = DrawShield; }

	void Setname(const string name) { m_name = name; }

	bool HPEffect();

	//攻撃時の瞬間取得
	void AttackCheck(const bool LastAttack);
	void AttackMove();
	//縮小する
	void ShrinkScale();
	//バウンド
	void BoundMove();
	//回復
	void RegeneUpdate();

	void ShieldUpdate();

	void ShieldTexUpdate();
	void PowerTexUpdate();

	static void HpPassive();
private:
	//三桁表示まで
	static const int NUMBER_MAX = 3;

	static const int DIR_MAX = 4;

	static bool isHpPassive;
	static float m_HP;
	static float m_MaxHP;

	static float startHP;
	static const int HEAL_MAX = 3;
	static const int SHIELD_NUM = 4;
public:
	//キャラの状態
	enum CharaState {
		STATE_MOVE,
		STATE_DELAY
	};
private:
	unique_ptr<IKETexture> shadow_tex;
	Input* input = Input::GetInstance();

	int _charaState = STATE_MOVE;

	//プレイヤーの現在位置
	XMFLOAT3 m_PanelPos = {};
	int m_NowHeight = {};
	int m_NowWidth = {};

	//イージング
	float m_Frame = {};
	int m_CoolTime = {};

	//入力フレーム
	int m_LimitCount = {};

	//方向
	enum SelectDir {
		DIR_UP,
		DIR_DOWN,
		DIR_RIGHT,
		DIR_LEFT
	};

	//各方向入力フレーム
	array<int, DIR_MAX> m_InputTimer;

	//グレイス用の変数
	float m_Length = {};
	float m_GrazeScore = {};
	XMFLOAT3 m_GrazePos = {};

	//HPの表示
	unique_ptr<IKESprite> hpCover;
	unique_ptr<IKESprite> hptex;
	unique_ptr<IKESprite> hpDiftex;
	unique_ptr<IKESprite> hptex_under;
	float m_OldHP = m_HP;
	float hp_frame = 0.f;
	float hp_wait = 0.f;
	bool isDamage = false;
	bool isHeal = false;
	//数値化したHP表示のための変数
	array<int, NUMBER_MAX> m_DigitNumber;
	int m_InterHP = {};//整数にしたHP
	array<int, NUMBER_MAX> m_DigitNumberMax;
	int m_InterMaxHP = {};//整数にしたHP

	XMFLOAT2 m_HPPos = { 85.0f,41.0f };
	XMFLOAT2 m_HPSize = { 554.f * 0.6f,54.f * 0.6f } ;
	//桁数
	enum DightType {
		FIRST_DIGHT,
		SECOND_DIGHT,
		THIRD_DIGHT
	};
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _drawnumber;
	std::vector<unique_ptr<DrawHealNumber>> _healnumber;
	unique_ptr<IKESprite> slash_;
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _MaxHp;

	bool m_Delay = false;
	bool m_Cancel = false;
	string m_name = "NONE";

	//影の変数
	XMFLOAT3 m_ShadowPos = {};
	XMFLOAT3 m_ShadowScale = {};
	bool is_title = false;

	//ダメージ関係
	bool m_Damege = false;
	int m_DamageTimer = {};
	int m_FlashCount = {};
	//移動関係
	bool m_Move = false;
	float m_MoveFrame = 0.0f;
	float m_AfterFrame = {};
	XMFLOAT3 m_AfterPos = {};

	enum MoveType {
		MOVE_NONE,//普通
		MOVE_EASE,//イージング
		MOVE_DISOLVE,//残像
	}_MoveType = MOVE_EASE;

	bool m_FinishGameOver = false;

	bool m_healingDamage = false;

	//攻撃時の動きのための変数
	float m_BaseScale = {};
	float m_AfterScale = 0.5f;
	int m_ShrinkTimer = {};

	enum AttackState {
		ATTACK_NONE,
		ATTACK_NORMAL,
		ATTACK_LAST,
	}_AttackState = ATTACK_NONE;

	float m_AttackFrame = {};

	bool m_Jump = false;
	//上昇度
	float m_AddPower = 0.0f;
	//重力加速度
	float m_Gravity = 0.02f;

	bool m_Bound = {};
	int m_HealTimer = {};
	bool m_SelectMove = false;

	int m_ClearTimer = {};
	float m_ClearFrame = {};

	float u_colorRad = 0.f;
	enum GameOverType {
		OVER_STOP,
		OVER_JUMP,
		OVER_MOVE,
		OVER_END
	}_OverType = OVER_STOP;

	int m_SelectType = {};

	bool m_SelectEnd = false;
	enum SelectType {
		NONE,
		SELECT_YES,
		SELECT_NO,
	};

	bool m_OverMove = false;

	int m_JumpCount = {};

	std::vector<float> healpower;
	struct Shield {
		std::unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 pos = {};
		float scale = 0.5f;
		XMFLOAT4 color = {};
		float CircleSpeed = {};
		float CircleScale = {};
	};
	struct Power {
		std::unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 pos = {};
		float scale = 0.5f;
		XMFLOAT4 color = {};
		float CircleSpeed = {};
		float CircleScale = {};
	};

	std::array<Shield, SHIELD_NUM> shield;
	std::array<Power, SHIELD_NUM> power;

	int m_ShieldTimer = {};
	float m_ShieldFrame = {};
	enum ShieldState {
		SHIELD_BIRTH,
		SHIELD_DELETE
	}_ShieldState = SHIELD_BIRTH;
	bool m_DrawShield = false;

	float m_PowerFrame = {};
	enum PowerState {
		POWER_BIRTH,
		POWER_DELETE
	}_PowerState = POWER_BIRTH;
	bool m_DrawPower = false;
};
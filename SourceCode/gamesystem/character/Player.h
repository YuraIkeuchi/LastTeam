#pragma once
#include "ObjCommon.h"
#include <IKESprite.h>
#include <IKETexture.h>
#include <memory>
#include <Input.h>
#include <array>
#include "DrawNumber.h"
#include "ImagePlayer.h"

using namespace DirectX;

class Player :
	public ObjCommon
{
private:
	static void (Player::* stateTable[])();
public:

	void InitState(const XMFLOAT3& pos);
	void LoadResource();
	//初期化
	bool Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon *dxCommon) override;
	void GameOverUpdate();
	//UI用
	void UIDraw();
	//ImGui
	void ImGuiDraw();
	//チュートリアルの更新
	void TitleUpdate();

	void SetTitleFlag(bool flag) { is_title = flag; }
private:
	//動き
	void Move();
	//動きが止まる
	void Delay();

	//パーティクル
	void BirthParticle();
	//HPの割合を求める
	float HpPercent();

public:
	//プレイヤーの回復
	void HealPlayer(const float power);
	//プレイヤーのダメージ
	void RecvDamage(const float Damage,const string& name = "NORMAL");
	//プレイヤーの情報保存
	void PlayerSave();
private:
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
	void BirthImagePlayer();
public:
	//getter setter
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
	const int GetCharaState() { return _charaState; }
	const bool GetDelay() { return m_Delay; }
	const bool GetFinishGameOver() { return m_FinishGameOver; }

	float GetMaxHp() { return m_MaxHP; }
	float GetHp() { return m_HP; }

	void SetMaxHp(float maxhp) {
		m_MaxHP = maxhp;
		m_HP = maxhp;
	}

	void SetGrazePos(const XMFLOAT3& GrazePos) { m_GrazePos = GrazePos; }

	void SetDelay(const bool Delay) { m_Delay = Delay; }

	void Setname(const string name) { m_name = name; }

	bool HPEffect();
private:
	//三桁表示まで
	static const int NUMBER_MAX = 3;

	static const int DIR_MAX = 4;

	static float startHP;
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


	//攻撃先
	XMFLOAT3 m_TargetPos = {};
	//戻り先
	XMFLOAT3 m_ReturnPos = {};
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
	unique_ptr<IKESprite> hptex;
	unique_ptr<IKESprite> hptex_under;
	float m_HP = {};
	float m_OldHP = m_HP;
	bool isDamage = false;
	bool isHeal = false;
	float m_MaxHP = {};
	//数値化したHP表示のための変数
	array<int, NUMBER_MAX> m_DigitNumber;
	int m_InterHP = {};//整数にしたHP
	array<int, NUMBER_MAX> m_DigitNumberMax;
	int m_InterMaxHP = {};//整数にしたHP

	XMFLOAT2 m_HPPos = { 10.f,5.0f };
	XMFLOAT2 m_HPSize = { 400.0f,40.0f };
	//桁数
	enum DightType {
		FIRST_DIGHT,
		SECOND_DIGHT,
		THIRD_DIGHT
	};
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _drawnumber;
	unique_ptr<IKESprite> slash_;
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _MaxHp;

	bool m_Delay = false;
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
	}_MoveType = MOVE_NONE;

	//残像用のプレイヤー
	std::vector<unique_ptr<ImagePlayer>> imageplayer;

	bool m_FinishGameOver = false;
};
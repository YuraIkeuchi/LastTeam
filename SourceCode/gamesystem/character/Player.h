#pragma once
#include "ObjCommon.h"
#include <IKESprite.h>
#include <memory>
#include <Input.h>
#include <array>
#include "DrawNumber.h"
using namespace DirectX;

class Player :public ObjCommon {
public:
	static Player* GetInstance();

private:
	static void (Player::* stateTable[])();
public:
	void InitState(const XMFLOAT3& pos);
	void LoadResource();
	//初期化
	bool Initialize()override;
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//UI用
	void UIDraw();
	//ImGui
	void ImGuiDraw();

private:
	//動き
	void Move();
	//動きが止まる
	void Delay();

	//パーティクル
	void BirthParticle();
	//HPの割合を求める
	float HpPercent();
private:
	void LoadCSV();

public:
	//getter setter
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
	const int GetCharaState() { return _charaState; }
	const bool GetDelayStart() { return m_DelayStart; }

	float GetMaxHp() { return m_MaxHP; }
	float GetHp() { return m_HP; }
	void SetMaxHp(float maxhp) {
		m_MaxHP = maxhp;
		m_HP = maxhp;
	}
	void SetGrazePos(const XMFLOAT3& GrazePos) { m_GrazePos = GrazePos; }

	void SetDelayTimer(const int DelayTimer) { m_DelayTimer = DelayTimer; }

	void Setname(const string name) { m_name = name; }

	void SetDelayStart(const bool DelayStart) { m_DelayStart = DelayStart; }
private:
	//三桁表示まで
	static const int NUMBER_MAX = 3;

	static const int DIR_MAX = 4;
public:
	//キャラの状態
	enum CharaState {
		STATE_MOVE,
		STATE_DELAY
	};
private:
	Input* input = Input::GetInstance();

	int _charaState = STATE_MOVE;
	//移動加算値
	float m_AddSpeed;
	XMFLOAT3 m_MoveRot = {};

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
	float m_HP = {};
	float m_MaxHP = {};
	//数値化したHP表示のための変数
	array<int, NUMBER_MAX> m_DigitNumber;
	int m_InterHP = {};//整数にしたHP

	XMFLOAT2 m_HPPos = { 100.0f,580.0f };
	XMFLOAT2 m_HPSize = { 400.0f,40.0f };
	//桁数
	enum DightType {
		FIRST_DIGHT,
		SECOND_DIGHT,
		THIRD_DIGHT
	};
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _drawnumber;

	int m_DelayTimer = {};

	bool m_DelayStart = false;

	string m_name = "NONE";
};
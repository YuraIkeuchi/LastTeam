#pragma once
#include "ObjCommon.h"
#include <any>
#include <list>
#include <Input.h>
#include <array>
using namespace DirectX;

class Player :public ObjCommon
{
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

	//ImGui
	void ImGuiDraw();

private:
	//動き
	void Move();

	//パーティクル
	void BirthParticle();

private:
	void LoadCSV();

public:
	//getter setter
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
	const int GetCharaState() { return _charaState; }

	void SetGrazePos(const XMFLOAT3& GrazePos) { m_GrazePos = GrazePos; }
private:

	static const int DIR_MAX = 4;
public:
	//キャラの状態
	enum CharaState
	{
		STATE_MOVE,
		STATE_ACTION
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
};
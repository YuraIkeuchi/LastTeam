#pragma once
#include "ObjCommon.h"
#include <any>
#include "ActionUI.h"
using namespace DirectX;
//行動の種類
enum ActType {
	ACT_ATTACK,
	ACT_GUARD,
	ACT_SKILL
};
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
	//行動UIの描画(あんまり良くないが一旦ここで)
	void ActUIDraw();
	//ImGui
	void ImGuiDraw();
	//行動力を入手する
	void AddAct(const string& Tag);
	//攻撃先指定
	void AttackTarget(const XMFLOAT3& pos);
private:
	//動き
	void Move();
	//攻撃
	void Attack();
	//移動
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);
	//行動UIの生成
	void BirthActUI(const string& Tag);
private:
	void LoadCSV();

public:
	//getter setter
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
	const int GetAllActCount() { return m_AllActCount; }
	const int GetCharaState() { return _charaState; }

private:
	static const int ACT_PATTERN = 3;
public:
	//キャラの状態
	enum CharaState
	{
		STATE_MOVE,
		STATE_ATTACK
	};
private:
	
	int _charaState = STATE_MOVE;
	//移動方向指定用
	float angle = 0.0f;
	//歩きまたは走り状態
	float velocity;
	//移動加算値
	float m_AddSpeed;
	XMFLOAT3 m_MoveRot = {};

	//プレイヤーの現在位置
	XMFLOAT3 m_PanelPos = {};
	int m_NowHeight = {};
	int m_NowWidth = {};

	//各行動回数
	int m_ActCount[ACT_PATTERN] = {};

	//全行動回数
	int m_AllActCount = {};
	//攻撃先
	XMFLOAT3 m_TargetPos = {};
	//戻り先
	XMFLOAT3 m_ReturnPos = {};
	//攻撃回数
	int m_AttackCount = {};
	//イージング
	float m_Frame = {};
	int m_CoolTime = {};

	enum AttackState {
		ATTACK_ENEMY,
		ATTACK_INTER,
	}_AttackState = ATTACK_ENEMY;
	
	vector<unique_ptr<ActionUI>> actui;
};
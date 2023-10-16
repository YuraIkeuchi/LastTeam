#pragma once
#include "ObjCommon.h"
#include <any>
#include "ActionUI.h"
#include "AttackArea.h"
#include <list>
#include <Input.h>
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
	//行動
	void SpecialAct();
	//行動の選択
	//void ChoiceAct();
	//攻撃
	void Attack();
	//防御
	void Guard();
	//スキルの行動
	void SkillAct();
	//行動UIの生成
	void BirthActUI(const string& Tag);
	//パーティクル
	void BirthParticle();
	//行動の終了
	void FinishAct();
	//攻撃エリアの描画
	void BirthArea();
	////押し続けているボタンキー取得
	//bool CheckButton(Input::XBOX Button,int& Timer);
	////ボタンを離した瞬間
	//bool LetCheckButton(Input::XBOX Button, int& Timer);
private:
	void LoadCSV();

public:
	//getter setter
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
	const int GetAllActCount() { return m_AllActCount; }
	const int GetCharaState() { return _charaState; }

	vector<AttackArea*>GetAttackArea() { return attackarea; }

private:
	static const int ACT_PATTERN = 3;
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
	//イージング
	float m_Frame = {};
	int m_CoolTime = {};

	enum AttackState {
		ATTACK_ENEMY,
		ATTACK_INTER,
	}_AttackState = ATTACK_ENEMY;
	
	//行動のUI
	vector<unique_ptr<ActionUI>> actui;
	//攻撃エリア
	vector<AttackArea*> attackarea;
	//行動先
	vector<int> m_Act;
	int m_AttackTimer = {};

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
};
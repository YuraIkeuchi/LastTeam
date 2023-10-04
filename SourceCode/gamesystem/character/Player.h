#pragma once
#include "ObjCommon.h"
#include <any>
using namespace DirectX;
class Player :public ObjCommon
{
public:
	static Player* GetInstance();

private:
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
	//行動力を入手する
	void AddAct(string Tag);
private:
	//動き
	void Move();
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);
private:
	void LoadCSV();

public:
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
private:
	static const int ACT_PATTERN = 3;
private:
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

	int m_ActCount[ACT_PATTERN] = {};

	enum ActType {
		ACT_ATTACK,
		ACT_GUARD,
		ACT_SKILL
	};
};
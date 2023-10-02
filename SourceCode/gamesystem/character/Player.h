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
	
private:
	XMFLOAT3 MoveVECTOR(XMVECTOR v, float angle);
private:
	void LoadCSV();

public:
	const int GetNowHeight() { return m_NowHeight; }
	const int GetNowWidth() { return m_NowWidth; }
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
};
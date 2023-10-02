#pragma once
#include "ObjCommon.h"
#include <any>
#include <array>
using namespace DirectX;
using namespace std;
class StagePanel
{
public:
	static StagePanel* GetInstance();

private:
	static const int PANEL_HEIGHT = 4;
	static const int PANEL_WIDTH = 4;

	static const int DIR_MAX = 4;
public:
	void LoadResource();
	//初期化
	bool Initialize();
	//更新
	void Update();
	//バトル中の更新
	void BattleUpdate();
	//スキルセットの更新
	void SetUpdate();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

public:
	array<array<unique_ptr<IKEObject3d>, PANEL_HEIGHT>, PANEL_WIDTH> m_Object;

	array<array<XMFLOAT3, PANEL_HEIGHT>, PANEL_WIDTH> m_Position;
	array<array<XMFLOAT4, PANEL_HEIGHT>, PANEL_WIDTH> m_Color;


	int m_SelectHeight = 0;
	int m_SelectWidth = 0;

	enum SelectDir {
		DIR_UP,
		DIR_DOWN,
		DIR_RIGHT,
		DIR_LEFT
	};

	int m_Timer[DIR_MAX] = {};
};
#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
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
	static const int PANEL_WIDTH = 8;

	static const int DIR_MAX = 4;
public:
	void LoadResource();
	//初期化
	bool Initialize();
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	//パネルの変更
	void PanelChange(const string& Tag);
	//パネルを戻す
	void DeletePanel();
private:
	//バトル中の更新
	void BattleUpdate();
	//スキルセットの更新
	void SetUpdate();
	void Collide();
public:
	//gettersetter
	const XMFLOAT3& GetSelectPos() { return m_SelectPos; }
	const bool GetCanSet() { return m_CanSet; }

	void SetSelectPos(const XMFLOAT3& position) { m_SelectPos = position; }
private:
	unique_ptr<IKEObject3d> m_Object[PANEL_WIDTH][PANEL_HEIGHT];
	XMFLOAT3 m_Position[PANEL_WIDTH][PANEL_HEIGHT] = {};
	XMFLOAT4 m_Color[PANEL_WIDTH][PANEL_HEIGHT] = {};

	//マスの位置
	int m_SelectHeight = 0;
	int m_SelectWidth = 0;

	//方向
	enum SelectDir {
		DIR_UP,
		DIR_DOWN,
		DIR_RIGHT,
		DIR_LEFT
	};

	//各方向入力フレーム
	array<int, DIR_MAX> m_Timer;

	XMFLOAT3 m_SelectPos = {};

	//パネルの種類
	enum PanelType {
		NO_PANEL,
		ATTACK_PANEL,
		GUARD_PANEL,
		SKILL_PANEL,
	};

	int m_PanelType[PANEL_WIDTH][PANEL_HEIGHT] = {};
	bool m_PanelHit[PANEL_WIDTH][PANEL_HEIGHT];
	
	//パネルを置けるかどうか
	bool m_CanSet = false;

	OBB m_OBB1 = {}, m_OBB2 = {};
};
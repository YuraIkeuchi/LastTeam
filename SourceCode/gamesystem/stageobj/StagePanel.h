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
public:
	void LoadResource();
	//èâä˙âª
	bool Initialize();
	//çXêV
	void Update();
	//ï`âÊ
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

public:
	array<array<unique_ptr<IKEObject3d>, PANEL_HEIGHT>, PANEL_WIDTH> m_Object;

	array<array<XMFLOAT3, PANEL_HEIGHT>, PANEL_WIDTH> m_Position;
	array<array<XMFLOAT4, PANEL_HEIGHT>, PANEL_WIDTH> m_Color;

	int m_SelectHeight = 0;
	int m_SelectWidth = 0;
};
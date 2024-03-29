﻿#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>
#include "DrawNumber.h"
using namespace std;         //  名前空間指定

class ActionUI {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	ActionUI(const int ID);

	//初期化
	void Initialize();
	//ステータスセット
	void InitState(const int ActCount,const int Type);
	//更新
	void Update();
	//UIの動き
	void UiMove();
	//描画
	void Draw();
	//ImGui
	void ImGuiDraw();
public:
	//gettersetter 
	const bool GetAlive() { return m_Alive; }
	const bool GetUse() { return m_Use; }
	void SetActCount(const int ActCount) { m_ActCount = ActCount; }
	void SetUse(const bool Use) { m_Use = Use; }

	int GetID() { return m_ID; }
	void SetID(const int ID) { m_ID = ID; }
private:
	static const int TEX_MAX = 3;
private:
	unique_ptr<IKESprite> tex;
	unique_ptr<IKESprite> vanishTexs[10];
	int vanishCount = 0;
	int vanishTiming = 0;
	XMFLOAT2 m_Position = {};
	XMFLOAT4 m_Color = { 1.3f,1.3f,1.3f,1.0f };
	XMFLOAT2 m_Size = { 64.f,64.f };

	int m_ActCount = {};

	int m_Type = {};
	bool m_Use = false;
	float m_Frame = {};
	bool m_Alive = true;

	int m_ID = {};
	unique_ptr<DrawNumber> _drawnumber;
};
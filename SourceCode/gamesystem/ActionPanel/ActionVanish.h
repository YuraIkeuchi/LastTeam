#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>
#include "DrawNumber.h"
using namespace std;         //  名前空間指定

class ActionVanish {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	ActionVanish();

	//初期化
	void Initialize();
	//ステータスセット
	void InitState();
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
private:
	unique_ptr<IKESprite> vanishTexs[10];
	int vanishCount = 0;
	int vanishTiming = 0;
	XMFLOAT2 m_Position = {};
	XMFLOAT4 m_Color = { 1.7f,1.7f,1.7f,1.0f };
	XMFLOAT2 m_Size = { 64.f,64.f };

	bool m_Alive = true;
};
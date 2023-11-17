#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>
#include "DrawNumber.h"
using namespace std;         //  名前空間指定

class DeckUI {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	DeckUI(const int ID);

	//初期化
	void Initialize();
	//ステータスセット
	void InitState(const int ActCount);
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
	void SetType(const int Type) { m_Type = Type; }
private:
	static const int TEX_MAX = 3;
private:
	unique_ptr<IKESprite> tex;

	XMFLOAT2 m_Position = {};
	XMFLOAT4 m_Color = { 1.3f,1.3f,1.3f,1.0f };
	int m_ActCount = {};

	int m_Type = {};
	bool m_Use = false;
	float m_Frame = {};
	bool m_Alive = true;

	int m_ID = {};
	unique_ptr<DrawNumber> _drawnumber;
};
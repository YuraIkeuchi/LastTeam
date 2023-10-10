#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>
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
	ActionUI();

	//初期化
	void Initialize();
	//ステータスセット
	void InitState(const int ActCount, const string& Tag);
	//更新
	void Update();
	//描画
	void Draw();
	//ImGui
	void ImGuiDraw();
private:
	static const int TEX_MAX = 3;
private:
	array<unique_ptr<IKESprite>,TEX_MAX> tex;

	XMFLOAT2 m_Position = {};
	int m_ActCount = {};

	int m_ActType = {};
};
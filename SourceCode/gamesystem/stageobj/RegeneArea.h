#pragma once
#include"DirectXCommon.h"
#include "IKETexture.h"
#include <DirectXMath.h>

using namespace std;         //  名前空間指定

//回復エリアクラス
class RegeneArea{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	RegeneArea();
public:
	//初期化
	bool Initialize();
	//ステータス初期化
	void InitState(const int width, const int height);
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
public:
	const bool GetAlive() { return m_Alive; }

	const int GetNowWidth() { return m_NowWidth; }
	const int GetNowHeight() { return m_NowHeight; }
private:
	//パネル
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		XMFLOAT3 scale = { 0.0f,0.0f,0.0f };
		bool predict = false;
	};
	float m_AfterScale = {};
	Panel panels;
	//生存関係
	bool m_Alive = false;
	int m_BirthTimer = 0;
	int m_NowWidth = {};
	int m_NowHeight = {};
};
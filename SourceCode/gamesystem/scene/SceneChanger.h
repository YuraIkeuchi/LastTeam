#pragma once
#include "Easing.h"
#include "IKESprite.h"
#include <memory>
#include <array>

using namespace std;         //  名前空間指定
using namespace DirectX;
//シーンチェンジクラス
class SceneChanger {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static SceneChanger* GetInstance();

	/// <summary>
	/// 初期k処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 初期k処理
	/// </summary>
	void InitializeOver();
	//描画
	void Draw();
	//ImGui
	void ImGuiDraw();

private:
	static void (SceneChanger::* stateTable[])();

	void FeedChange();
	void WideChange();
	XMFLOAT4 ChangeColor(const int Weight, const int Height);
private:
	static const int DOOR_NUM = 2;
public:
	const bool GetChange() { return m_Change; }
	const bool GetChangeStart() { return m_ChangeStart; }
	void SetChange(const bool Change) { m_Change = Change; }
	void SetChangeStart(const bool ChangeStart) { m_ChangeStart = ChangeStart; }

private:
	static const int HEIGHT_NUM = 9;
	static const int WIDTH_NUM = 16;
private:
	//シーンチェンジのタイプ
	enum ChangeType {
		CHANGE_FEED,
		CHANGE_WIDE,
	}_ChangeType = CHANGE_WIDE;

	//普通のフェード用
	unique_ptr<IKESprite> change;
	float m_Frame = {};

	bool m_ChangeStart = false;
	bool m_Change = false;
	int m_ChangeTimer = {};

	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,0.0f };
	float m_AfterAlpha = {};

	//広がるよう
	const float width = WinApp::window_width;
	const float height = WinApp::window_height;
	const int base_size = 80;

	unique_ptr<IKESprite> change2[WIDTH_NUM][HEIGHT_NUM];
	XMFLOAT4 m_Color2[WIDTH_NUM][HEIGHT_NUM];
	
	XMFLOAT2 m_WideSize = {};
	float m_AfterSize = {};
	//シーンチェンジ状態
	enum CHANGESTATE {
		CHANGE_START,
		CHANGE_FINISH
	}_ChangeState;

public:
	int GetChangeState() { return _ChangeState; }
};
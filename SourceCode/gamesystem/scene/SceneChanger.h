#pragma once
#include "Easing.h"
#include "IKESprite.h"
#include <memory>
#include <array>

using namespace std;         //  名前空間指定
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
	static const int DOOR_NUM = 2;
public:
	const bool GetChange() { return m_Change; }
	const bool GetChangeStart() { return m_ChangeStart; }
	void SetChange(const bool Change) { m_Change = Change; }
	void SetChangeStart(const bool ChangeStart) { m_ChangeStart = ChangeStart; }
private:
	unique_ptr<IKESprite> change;
	float m_Frame = {};

	bool m_ChangeStart = false;
	bool m_Change = false;
	int m_ChangeTimer = {};

	enum FEEDSTATE {
		FEED_START,
		FEED_FINISH
	}_FeedState;

	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,0.0f };
	float m_AfterAlpha = {};
};
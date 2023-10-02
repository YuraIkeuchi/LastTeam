#pragma once
#include "TextManager.h"
#include "IKESprite.h"
using namespace std;         //  名前空間指定
enum TextState {
	TEXTANGER_START,
	TEXTANGER_SECOND,
	TEXTJOY_START,
	TEXTJOY_SECOND,
	TEXTJOY_THIRD,
	TEXTSELECT_EMO
};
//敵の管理系クラス
class BossText {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	BossText();
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//描画
	void SpriteDraw(DirectXCommon* dxCommon);
	//メッセージ画面
	void Display();
	//メッセージ選択
	void ChangeColor(int row, const XMVECTOR& color);
	void ChangeColor(const XMVECTOR& color);

public:
	void SetTextState(const int TextState) { m_TextState = TextState; }
	void SetWindowInvisible() { this->window_invisible_ = false; }
private:
	unique_ptr<TextManager> text_;
	unique_ptr<IKESprite> window_;

	//カメラの状態
	int m_TextState = 0;

	//Windows関係
	XMFLOAT2 window_pos{ WinApp::window_width / 2.f,WinApp::window_height + 100 };
	XMFLOAT2 window_size{ 0.f,0.f };
	float m_Frame = 0.0f;
	bool window_invisible_ = true;
};
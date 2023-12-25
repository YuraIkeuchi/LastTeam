#pragma once
#include "BaseScene.h"
#include"BossText.h"
#include "MessageWindow.h"
#include"InterEnemy.h"

//ゲームプレイシーン
class TutorialScene : public BaseScene
{
public:
	~TutorialScene() override {};
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//開放
	void Finalize() override;

private:
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw();
	//void TutorialManager();//チュートリアルの進行状況

	static void (TutorialScene::* stateTable[])();
	void PlayState();//チュートリアル動かし
	void TutorialEnd();//終わり
	void Skip();		//スキップ

private:
	unique_ptr<TextManager> text_;
	unique_ptr<InterEnemy> enemy = nullptr;

	unique_ptr<IKESprite> ui = nullptr;
	unique_ptr<IKESprite> skipUI = nullptr;
	unique_ptr<IKESprite> skipUnder = nullptr;
	unique_ptr<IKESprite> skipBack = nullptr;
	float scale_skip = 0.7f;
	float frame = 0.f;
	enum ChangeType {
		CHANGE_TITLE,
		CHANGE_OVER,
	}_ChangeType = CHANGE_TITLE;

	int m_Timer = {};

	enum TutorialState {
		TUTORIAL_PLAY,
		TUTORIAL_FINISH,
	}_nowstate = TUTORIAL_PLAY;

	bool m_End = false;
	bool m_Skip = false;
	bool m_IsBackKey = false;
	int m_TextTimer = {};
	struct Window {
		unique_ptr<IKESprite> sprite;
		XMFLOAT2 m_Size = {};
		XMFLOAT2 m_Pos = {};
		float m_Frame = {};
	};

	Window window;
	XMFLOAT2 m_TextPos = { -60.0f,-30.f };
};
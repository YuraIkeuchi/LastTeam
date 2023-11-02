#pragma once
#include "BaseScene.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "EnemyManager.h"

//ゲームプレイシーン
class TutorialScene : public BaseScene
{
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
	void IntroState();
	void MoveState();	//動く
	void GetState();	//スキルゲット
	void AttackState();		//攻撃
	void DamageState();//ダメージを与える
	void TutorialEnd();//終わり
	void Skip();		//スキップ

private:
	std::array<unique_ptr<Font>, 3> texts;
	unique_ptr<EnemyManager> enemyManager = nullptr;

	unique_ptr<IKESprite> ui = nullptr;

	enum ChangeType {
		CHANGE_TITLE,
		CHANGE_OVER,
	}_ChangeType = CHANGE_TITLE;

	int m_Timer = {};

	enum TutorialState {
		TUTORIAL_INTRO,
		TUTORIAL_MOVE,
		TUTORIAL_GETSKILL,
		TUTORIAL_ATTACK,
		TUTORIAL_DAMAGE,
		TUTORIAL_FINISH,
	}_nowstate = TUTORIAL_INTRO;

	bool m_End = false;
};
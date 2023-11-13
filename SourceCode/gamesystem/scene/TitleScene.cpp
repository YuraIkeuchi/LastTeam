#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <Player.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include "MobEnemy.h"
#include "TutorialTask.h"
#include "GameStateManager.h"
// 遷移しうるシーン
#include "BattleScene.h"
#include "MapScene.h"
#include "TutorialScene.h"
#include "TextManager.h"
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TITLE);
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}


	//�X�e�[�W�̏�
	// プレイヤー生成
	{
		auto player = GameObject::CreateObject<Player>();	// �v���C���[����
		player->LoadResource();
		player->InitState({ -4.0f,0.1f,2.0f });
		player->Initialize();
		player->SetTitleFlag(true);

		GameStateManager::GetInstance()->SetPlayer(player);
	}

	StagePanel::GetInstance()->LoadResource();
	
	GameReset({ -4.0f,0.1f,2.0f });

	//敵
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();

	//カード
	title_[TITLE_BACK] = IKESprite::Create(ImageManager::TITLEBACK, { 0.0f,0.0f });
	title_[TITLE_TEXT] = IKESprite::Create(ImageManager::TITLETEXT, { 640.0f,200.0f },{1.0f,1.0f,1.0f,1.0f},{0.5f,0.5f});

}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera);
	// 全オブジェクト更新
	game_object_manager_->Update();

	//各クラス更新
	camerawork->Update(camera);

	lightGroup->Update();
	game_object_manager_->Update();
	//Player::GetInstance()->TitleUpdate();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();

	if ((input->TriggerButton(input->B))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
		//チュートリアルのタスク
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);

	}
	if (input->TriggerKey(DIK_SPACE)) {			//マップ
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = MAP;
		//チュートリアルのタスク
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
	}
	if (input->TriggerButton(input->X)) {			//チュートリアル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = TUTORIAL;
		//チュートリアルのタスク
		TutorialTask::GetInstance()->SetTutorialState(TASK_MOVE);
	}
	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		SceneManager::GetInstance()->ChangeScene<MapScene>();
		SceneChanger::GetInstance()->SetChange(false);
	}

}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//前面描画
void TitleScene::FrontDraw(DirectXCommon* dxCommon) {
	text_->TestDraw(dxCommon);
	IKESprite::PreDraw();
	title_[TITLE_TEXT]->Draw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();

}
//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	title_[TITLE_BACK]->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	game_object_manager_->Draw(dxCommon);

	IKEObject3d::PostDraw();

	enemy->Draw(dxCommon);

}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	camerawork->ImGuiDraw();
	game_object_manager_->ImGuiDraw();
}
//解放
void TitleScene::Finalize() {
}

#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <StagePanel.h>
#include "MobEnemy.h"
#include "GameStateManager.h"
#include "TextManager.h"
#include <StageBack.h>
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}

	nowHierarchy = 0;
	clearHierarchy = 1;
	nowIndex = 1;
	s_LastStage = false;

	////�X�e�[�W�̏�
//// プレイヤー生成
//{
//	auto player = GameObject::CreateObject<Player>();	// �v���C���[����
//	player->LoadResource();
//	player->InitState({ -4.0f,0.1f,2.0f });
//	player->Initialize();
//	player->SetTitleFlag(true);

//	GameStateManager::GetInstance()->SetPlayer(player);
//}
	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->SetTitleFlag(true);
	player_->InitState({ -PANEL_SIZE * 2.f,0.1f,PANEL_SIZE });
	player_->Initialize();
	player_->SkipInitialize();
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	StagePanel::GetInstance()->Initialize();
	//背景画像
	StageBack::GetInstance()->LoadResource();

	////敵
	InterEnemy::SetPlayer(player_.get());
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();

	//カード
	title_ = IKESprite::Create(ImageManager::TITLETEXT, { 0.0f,0.0f });
	GameStateManager::GetInstance()->DeckReset();
	GameStateManager::GetInstance()->SetGameStart(true);
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera);
	player_->Update();
	//各クラス更新
	camerawork->Update(camera);

	lightGroup->Update();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();

	if ((input->TriggerButton(input->B)|| input->TriggerKey(DIK_SPACE)) && (!SceneChanger::GetInstance()->GetChangeStart())) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
	}
	
	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		player_->PlayerSave();
		SceneManager::GetInstance()->ChangeScene("CLEAR");
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
	IKESprite::PreDraw();
	title_->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();

}
//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	//title_[TITLE_BACK]->Draw();
	StageBack::GetInstance()->Draw(dxCommon);
	IKESprite::PostDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	player_->Draw(dxCommon);
	enemy->Draw(dxCommon);

}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	//camerawork->ImGuiDraw();
	//player_->ImGuiDraw();
}
//解放
void TitleScene::Finalize() {
}

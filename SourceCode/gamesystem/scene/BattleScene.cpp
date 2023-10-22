#include "BattleScene.h"
#include <Player.h>
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameMode.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
//初期化
void BattleScene::Initialize(DirectXCommon* dxCommon)
{
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ -8.0f,1.0f,0.0f });
	Player::GetInstance()->Initialize();

	//ステージの床
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();

	//ゲームモード
	GameMode::GetInstance()->Initialize();

	//ゲームの状態
	GameStateManager::GetInstance()->Initialize();

	//敵
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();
}
//更新
void BattleScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	// 全オブジェクト更新
	game_object_manager_->Update();

	//各クラス更新
	camerawork->Update(camera);
	lightGroup->Update();
	Player::GetInstance()->Update();
	StagePanel::GetInstance()->Update();
	GameMode::GetInstance()->Update();
	GameStateManager::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemyManager->Update();
	enemyManager->GetCameraData(camera->GetViewMatrix(), camera->GetProjectionMatrix(), camera->GetViewPort());
	//敵を倒したらシーン以降(仮)
	if (enemyManager->BossDestroy()) {
		SceneManager::GetInstance()->PopScene();
	}
}

void BattleScene::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw();
		dxCommon->PostDraw();
	}
}
//ポストエフェクトかからない
void BattleScene::FrontDraw(DirectXCommon* dxCommon) {
	ParticleEmitter::GetInstance()->FlontDrawAll();
	Player::GetInstance()->ActUIDraw();
	enemyManager->UIDraw();
	SceneChanger::GetInstance()->Draw();
}
//ポストエフェクトかかる
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemyManager->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	//camerawork->ImGuiDraw();
	Player::GetInstance()->ImGuiDraw();
	StagePanel::GetInstance()->ImGuiDraw();
	//GameMode::GetInstance()->ImGuiDraw();
	enemyManager->ImGuiDraw();
	GameStateManager::GetInstance()->ImGuiDraw();
	SceneChanger::GetInstance()->ImGuiDraw();
}

void BattleScene::Finalize() {

}
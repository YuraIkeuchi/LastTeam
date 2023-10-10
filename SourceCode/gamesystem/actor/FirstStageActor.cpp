#include "FirstStageActor.h"
#include"Easing.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include "Player.h"
#include "StagePanel.h"
#include "Helper.h"
#include "FPSManager.h"
#include "GameMode.h"
void FirstStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	//Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	m_SceneState = SceneState::IntroState;

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	//地面
	ground.reset(new IKEObject3d());
	ground->Initialize();
	ground->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::GROUND));
	ground->SetScale({ 2.f,1.f,2.f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetAddOffset(3.0f);
	ground->VertexCheck();

	//スカイドーム
	skydome.reset(new IKEObject3d());
	skydome->Initialize();
	skydome->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::SKYDOME));
	skydome->SetScale({ 8.f,8.f,8.f });
	skydome->SetPosition({ 0.0f,0.0f,0.0f });
	skydome->VertexCheck();

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ -5.0f,1.0f,0.0f });
	Player::GetInstance()->Initialize();

	//ステージの床
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();

	//ゲームモード
	GameMode::GetInstance()->Initialize();

	//敵
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();

	//行動のUI
	actui = make_unique<ActionUI>();
	actui->Initialize();
	//テクスチャ
	tex.reset(IKETexture::Create(ImageManager::MAGIC, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	tex->TextureCreate();
	tex->SetPosition({ 5.0f,2.0f,0.0f });
	tex->SetScale({ 0.5f,0.5f,0.5f });
	tex->SetIsBillboard(true);
	tex->SetColor({ 1.0f,0.0,0.0f,1.0f });

}

void FirstStageActor::Finalize() {
}

void FirstStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);

	//各クラス更新
	camerawork->Update(camera);
	lightgroup->Update();
	ground->Update();
	skydome->Update();
	m_AddOffset.x = 0.001f;
	ground->SetAddOffset(m_AddOffset.x);
	Player::GetInstance()->Update();
	StagePanel::GetInstance()->Update();
	GameMode::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	enemyManager->Update();
	tex->Update();
	actui->Update();
}

void FirstStageActor::Draw(DirectXCommon* dxCommon) {
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
	} else {
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
void FirstStageActor::FrontDraw(DirectXCommon* dxCommon) {
	ParticleEmitter::GetInstance()->FlontDrawAll();
	actui->Draw();
}
//ポストエフェクトかかる
void FirstStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemyManager->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//導入しーんの更新
void FirstStageActor::IntroUpdate(DebugCamera* camera) {

}
//プレイ中の更新
void FirstStageActor::MainUpdate(DebugCamera* camera) {

}
//クリア後の更新
void FirstStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}
//ImGui
void FirstStageActor::ImGuiDraw() {
	camerawork->ImGuiDraw();
	Player::GetInstance()->ImGuiDraw();
	StagePanel::GetInstance()->ImGuiDraw();
	GameMode::GetInstance()->ImGuiDraw();
	enemyManager->ImGuiDraw();
}

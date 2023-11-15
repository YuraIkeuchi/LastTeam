#include "BattleScene.h"

#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <SceneChanger.h>

#include "InterEnemy.h"
BattleScene::~BattleScene() {
	Finalize();
}
// 初期化
void BattleScene::Initialize(DirectXCommon* dxCommon)
{
	//シーンの基本初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	// ポストエフェクト
	PlayPostEffect = false;
	
	// パーティクル
	ParticleEmitter::GetInstance()->AllDelete();

	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ -4.0f,0.1f,2.0f });
	player_->Initialize();
	//ゲームステート初期化
	GameStateManager::GetInstance()->SetDxCommon(dxCommon);
	GameStateManager::GetInstance()->Initialize();
	GameStateManager::SetPlayer(player_.get());
	//ステージパネルの初期化
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	//ビヘイビア試しました！
	{
		//auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
	}

	//敵
	EnemyManager::SetPlayer(player_.get());
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();
	
	//パッシブスキルによるエネミーの能力変更
	if (GameStateManager::GetInstance()->GetPoisonSkill()) {
		enemyManager->PoizonGauge();
	}
	if (GameStateManager::GetInstance()->GetPoisonVenom()) {
		enemyManager->PoizonVenom();
	}

	GameReset({ -8.0f,0.1f,0.0f });
	StagePanel::GetInstance()->DeleteAction();
}
//更新
void BattleScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	//ライト更新
	lightGroup->Update();
	//�e�N���X�X�V
	//カメラワーク更新
	camerawork->Update(camera);
	player_->Update();
	GameStateManager::GetInstance()->Update();

	StagePanel::GetInstance()->Update();
	enemyManager->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	//エネミーが全員死亡したら
	if (enemyManager->BossDestroy()) {
		//クリア処理が終らなかったら
		if (!GameStateManager::GetInstance()->GetIsChangeScene()) {
			//クリア処理準備
			GameStateManager::GetInstance()->StageClearInit();
		} else {
			//マップに戻る
			_ChangeType = CHANGE_MAP;
			SceneChanger::GetInstance()->SetChangeStart(true);
		}
	}
	//クリア条件に達するとプレイヤーを動かせなくする
	if (GameStateManager::GetInstance()->GetIsFinish()) {
		player_->SetDelay(true);
	}
	//ゲームオーバー処理
	if (player_->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	//シーン切り替え処理
	if (SceneChanger::GetInstance()->GetChange()) {
		GameReset({ -4.0f,0.1f,2.0f });
		if (_ChangeType == CHANGE_MAP) {
			if (!s_LastStage) {
				SceneManager::GetInstance()->ChangeScene("MAP");
			}
			else {
				SceneManager::GetInstance()->ChangeScene("CLEAR");
			}
		}else {
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
		player_->PlayerSave();
		SceneChanger::GetInstance()->SetChange(false);
	}
}

void BattleScene::Draw(DirectXCommon* dxCommon) {
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

//前方描画(奥に描画するやつ)
void BattleScene::FrontDraw(DirectXCommon* dxCommon) {
	ParticleEmitter::GetInstance()->FlontDrawAll();

	if (!enemyManager->BossDestroy()){
		player_->UIDraw();
		enemyManager->UIDraw();
		GameStateManager::GetInstance()->ActUIDraw();
	}
	SceneChanger::GetInstance()->Draw();
}
//後方描画(主にSprite)
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	player_->Draw(dxCommon);

	GameStateManager::GetInstance()->Draw(dxCommon);
	enemyManager->Draw(dxCommon);
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	//StagePanel::GetInstance()->ImGuiDraw();
	//GameStateManager::GetInstance()->ImGuiDraw();
	player_->ImGuiDraw();
	//game_object_manager_->ImGuiDraw();
	enemyManager->ImGuiDraw();
}

void BattleScene::Finalize() {
	
}
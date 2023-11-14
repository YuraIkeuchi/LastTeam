#include "BattleScene.h"

#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>

#include "Player.h"
#include "BaseEnemy.h"
#include "InterEnemy.h"
#include "GameoverScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
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
	// プレイヤー生成
	{
		auto player = GameObject::CreateObject<Player>();	// �v���C���[����
		player->LoadResource();
		player->Initialize();
		player->InitState({ -8.0f,0.1f,0.0f });
		GameStateManager::GetInstance()->SetPlayer(player);
	}
	//ゲームステート初期化
	GameStateManager::GetInstance()->SetDxCommon(dxCommon);
	GameStateManager::GetInstance()->Initialize();
	//ステージパネルの初期化
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();

	//ビヘイビア試しました！
	{
		//auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
	}

	//敵
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();
	//enemyManager->EnemyLightInit(lightGroup);
	
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
	StagePanel::GetInstance()->Update();
	// �S�I�u�W�F�N�g�X�V
	game_object_manager_->Update();
	GameStateManager::GetInstance()->Update();


	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemyManager->Update();
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
		auto player = GameStateManager::GetInstance()->GetPlayer();
		player.lock()->SetDelay(true);
	}
	//ゲームオーバー処理
	if (GameStateManager::GetInstance()->GetPlayer().lock()->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	//シーン切り替え処理
	if (SceneChanger::GetInstance()->GetChange()) {
		GameReset({ -4.0f,0.1f,2.0f });
		if (_ChangeType == CHANGE_MAP) {
			if (!s_LastStage) {
				SceneManager::GetInstance()->PopScene();
			}
			else {
				SceneManager::GetInstance()->ChangeScene<ClearScene>();
			}
		}else {
			SceneManager::GetInstance()->ChangeScene<GameoverScene>();
		}
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
		game_object_manager_->UIDraw();
		enemyManager->UIDraw();
		GameStateManager::GetInstance()->ActUIDraw();
	}
	SceneChanger::GetInstance()->Draw();
}
//後方描画(主にSprite)
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	game_object_manager_->Draw(dxCommon);

	GameStateManager::GetInstance()->Draw(dxCommon);
	enemyManager->Draw(dxCommon);
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	GameStateManager::GetInstance()->ImGuiDraw();
	//game_object_manager_->ImGuiDraw();
	//enemyManager->ImGuiDraw();
}

void BattleScene::Finalize() {
	game_object_manager_->Finalize();
}
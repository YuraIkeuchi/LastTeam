#include "BattleScene.h"
#include <Player.h>
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include "SkillManager.h"
#include "GameoverScene.h"
#include "TitleScene.h"

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
	Player::GetInstance()->InitState({ -8.0f,0.1f,0.0f });
	Player::GetInstance()->Initialize();
	//スキル
	SkillManager::GetInstance()->Initialize();
	//ゲームの状態
	GameStateManager::GetInstance()->Initialize();
	//ステージの床
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();

	//敵
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();

	//リザルトテキスト
	resulttext = make_unique<TextManager>();
	resulttext->Initialize(dxCommon);
	resulttext->SetConversation(TextManager::RESULT, { 5.0f,280.0f });

	//丸影
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
}
//更新
void BattleScene::Update(DirectXCommon* dxCommon)
{
	//プレイヤー
	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.5f, Player::GetInstance()->GetPosition().z }));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	lightGroup->Update();
	// 全オブジェクト更新
	game_object_manager_->Update();

	//各クラス更新
	camerawork->Update(camera);
	if (!GameStateManager::GetInstance()->GetIsFinish()) {
		Player::GetInstance()->Update();
	}
	StagePanel::GetInstance()->Update();
	GameStateManager::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemyManager->Update();
	enemyManager->SetLight(lightGroup);
	//敵を倒したらシーン以降(仮)
	if (enemyManager->BossDestroy()) {
		if (!GameStateManager::GetInstance()->GetIsChangeScene()) {
			GameStateManager::GetInstance()->StageClearInit();
		} else {
			_ChangeType = CHANGE_TITLE;
			SceneChanger::GetInstance()->SetChangeStart(true);
		}
	}
	//ぷれいやーのHPが無くなっても遷移する
	if (Player::GetInstance()->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		if (_ChangeType == CHANGE_TITLE) {
			SceneManager::GetInstance()->ChangeScene<TitleScene>(true);
		}
		else {
			SceneManager::GetInstance()->ChangeScene<GameoverScene>();
		}
		SceneChanger::GetInstance()->SetChange(false);
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
	if (enemyManager->BossDestroy()) {
		resulttext->TestDraw(dxCommon);
	}
	Player::GetInstance()->UIDraw();
	enemyManager->UIDraw();
	GameStateManager::GetInstance()->ActUIDraw();
	SceneChanger::GetInstance()->Draw();
}
//ポストエフェクトかかる
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemyManager->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	GameStateManager::GetInstance()->ImGuiDraw();
	//SceneChanger::GetInstance()->ImGuiDraw();
	Player::GetInstance()->ImGuiDraw();
	enemyManager->ImGuiDraw();
}

void BattleScene::Finalize() {

}
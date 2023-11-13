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

// 初期化
void BattleScene::Initialize(DirectXCommon* dxCommon)
{
	//
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	// ポストエフェクト
	PlayPostEffect = false;
	
	// パテーィクル
	ParticleEmitter::GetInstance()->AllDelete();

	
	//�X�L��
	// プレイヤー生成
	{
		auto player = GameObject::CreateObject<Player>();	// �v���C���[����
		player->LoadResource();
		player->InitState({ -8.0f,0.1f,0.0f });

		GameStateManager::GetInstance()->SetPlayer(player);

	}
	//�Q�[���̏��
	GameStateManager::GetInstance()->Initialize();
	//���U���g�e�L�X�g
	//�X�e�[�W�̏�
	StagePanel::GetInstance()->LoadResource();

	StagePanel::GetInstance()->Initialize();


	// �G�l�~�[
	{
		auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
	}

	//リザルトテキスト
	resulttext = make_unique<TextManager>();
	resulttext->Initialize(dxCommon);
	resulttext->SetConversation(TextManager::RESULT, { 5.0f,280.0f });

	//敵
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();
	//enemyManager->EnemyLightInit(lightGroup);

	if (GameStateManager::GetInstance()->GetPoisonSkill()) {
		enemyManager->PoizonGauge();
	}
	if (GameStateManager::GetInstance()->GetPoisonVenom()) {
		enemyManager->PoizonVenom();
	}

	GameReset({ -8.0f,0.1f,0.0f });
}
//�X�V
void BattleScene::Update(DirectXCommon* dxCommon)
{
	lightGroup->Update();
	// �S�I�u�W�F�N�g�X�V
	game_object_manager_->Update();

	//�e�N���X�X�V
	camerawork->Update(camera);
	if (!GameStateManager::GetInstance()->GetIsFinish()) {
		// Player::GetInstance()->Update();
	}
	StagePanel::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemyManager->Update();
	//enemyManager->EnemyLightUpdate(lightGroup);
	GameStateManager::GetInstance()->Update();
	//�G���|�������V�[���ȍ~(��)
	if (enemyManager->BossDestroy()) {
		if (!GameStateManager::GetInstance()->GetIsChangeScene()) {
			GameStateManager::GetInstance()->StageClearInit();
		} else {
			_ChangeType = CHANGE_MAP;
			SceneChanger::GetInstance()->SetChangeStart(true);
		}
	}
	//�Ղꂢ��[��HP�������Ȃ��Ă��J�ڂ���
	if (GameStateManager::GetInstance()->GetPlayer().lock()->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		GameReset({ -4.0f,0.1f,2.0f });
		if (_ChangeType == CHANGE_MAP) {
			SceneManager::GetInstance()->PopScene();
		}
		else {
			SceneManager::GetInstance()->ChangeScene<GameoverScene>();
		}
		SceneChanger::GetInstance()->SetChange(false);
	}
}

void BattleScene::Draw(DirectXCommon* dxCommon) {
	//�`����@
	//�|�X�g�G�t�F�N�g�������邩
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
//�|�X�g�G�t�F�N�g������Ȃ�
void BattleScene::FrontDraw(DirectXCommon* dxCommon) {
	ParticleEmitter::GetInstance()->FlontDrawAll();

	game_object_manager_->UIDraw();

	if (enemyManager->BossDestroy()) {
		resulttext->TestDraw(dxCommon);
	}
	if (!enemyManager->BossDestroy()){
		enemyManager->UIDraw();
		GameStateManager::GetInstance()->ActUIDraw();
	}
	// Player::GetInstance()->UIDraw();
	enemyManager->UIDraw();

	GameStateManager::GetInstance()->ActUIDraw();
	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	game_object_manager_->Draw(dxCommon);

	StagePanel::GetInstance()->Draw(dxCommon);
	GameStateManager::GetInstance()->Draw(dxCommon);
	enemyManager->Draw(dxCommon);
	IKEObject3d::PostDraw();

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	GameStateManager::GetInstance()->ImGuiDraw();
	game_object_manager_->ImGuiDraw();
	//SceneChanger::GetInstance()->ImGuiDraw();
	enemyManager->ImGuiDraw();
}

void BattleScene::Finalize() {

}
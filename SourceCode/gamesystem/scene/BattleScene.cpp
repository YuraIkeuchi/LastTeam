#include "BattleScene.h"

#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>

#include "SkillManager.h"
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

	// プレイヤー生成
	{
		auto player = GameObject::CreateObject<Player>();	// �v���C���[����
		player->LoadResource();
		player->InitState({ -8.0f,1.0f,0.0f });
		player->Initialize();

		GameStateManager::GetInstance()->SetPlayer(player);

	}
	//�X�L��
	SkillManager::GetInstance()->Initialize();
	//�Q�[���̏��
	GameStateManager::GetInstance()->Initialize();
	//�X�e�[�W�̏�
	StagePanel::GetInstance()->LoadResource();

	// �G�l�~�[
	{
		auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
		test_enemy_1->Initialize();
	}

	//�Q�[���̏��
	GameStateManager::GetInstance()->Initialize();

	//�X�L��
	SkillManager::GetInstance()->Initialize();
	StagePanel::GetInstance()->Initialize();

	
	//�G
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();
}
//�X�V
void BattleScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	// �S�I�u�W�F�N�g�X�V
	game_object_manager_->Update();

	//�e�N���X�X�V
	camerawork->Update(camera);
	lightGroup->Update();
	StagePanel::GetInstance()->Update();
	GameStateManager::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	//enemyManager->Update();
	//�G��|������V�[���ȍ~(��)
	if (enemyManager->BossDestroy()) {
		if (!GameStateManager::GetInstance()->GetIsChangeScene()) {
			GameStateManager::GetInstance()->StageClearInit();
		} else {
			_ChangeType = CHANGE_TITLE;
			SceneChanger::GetInstance()->SetChangeStart(true);
		}
	}
	//�Ղꂢ��[��HP�������Ȃ��Ă��J�ڂ���
	if (GameStateManager::GetInstance()->GetPlayer().lock()->GetHp() <= 0.0f) {
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

	SceneChanger::GetInstance()->Draw();
	//enemyManager->UIDraw();
	GameStateManager::GetInstance()->ActUIDraw();
	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	game_object_manager_->Draw();

	StagePanel::GetInstance()->Draw(dxCommon);
	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	//enemyManager->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void BattleScene::ImGuiDraw() {
	GameStateManager::GetInstance()->ImGuiDraw();
	game_object_manager_->ImGuiDraw();
	//SceneChanger::GetInstance()->ImGuiDraw();
}

void BattleScene::Finalize() {

}
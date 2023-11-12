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

//������
void BattleScene::Initialize(DirectXCommon* dxCommon)
{
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//�|�X�g�G�t�F�N�g
	PlayPostEffect = false;
	
	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();

	//�v���C���[
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ -8.0f,0.1f,0.0f });
	Player::GetInstance()->Initialize();
	//�X�L��
	SkillManager::GetInstance()->Initialize();
	//�Q�[���̏��
	GameStateManager::GetInstance()->Initialize();

	//���U���g�e�L�X�g
	resulttext = make_unique<TextManager>();
	resulttext->Initialize(dxCommon);
	resulttext->SetConversation(TextManager::RESULT, { 5.0f,280.0f });

	//�G
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();
	//enemyManager->EnemyLightInit(lightGroup);

	if (GameStateManager::GetInstance()->GetPoisonSkill()) {
		enemyManager->PoizonGauge();
	}
	if (GameStateManager::GetInstance()->GetPoisonVenom()) {
		enemyManager->PoizonVenom();
	}
	
	//�X�e�[�W�̏�
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();
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
		Player::GetInstance()->Update();
	}
	StagePanel::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemyManager->Update();
	//enemyManager->EnemyLightUpdate(lightGroup);
	GameStateManager::GetInstance()->Update();
	//�G��|������V�[���ȍ~(��)
	if (enemyManager->BossDestroy()) {
		if (!GameStateManager::GetInstance()->GetIsChangeScene()) {
			GameStateManager::GetInstance()->StageClearInit();
		} else {
			_ChangeType = CHANGE_MAP;
			SceneChanger::GetInstance()->SetChangeStart(true);
		}
	}
	//�Ղꂢ��[��HP�������Ȃ��Ă��J�ڂ���
	if (Player::GetInstance()->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
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
	if (enemyManager->BossDestroy()) {
		resulttext->TestDraw(dxCommon);
	}
	if (!enemyManager->BossDestroy()){
		Player::GetInstance()->UIDraw();
		enemyManager->UIDraw();
		GameStateManager::GetInstance()->ActUIDraw();
	}
	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
void BattleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemyManager->Draw(dxCommon);
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
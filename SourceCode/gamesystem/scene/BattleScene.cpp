#include "BattleScene.h"
#include <Player.h>
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameMode.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
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
	Player::GetInstance()->InitState({ -8.0f,1.0f,0.0f });
	Player::GetInstance()->Initialize();

	//�X�e�[�W�̏�
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();

	//�Q�[�����[�h
	GameMode::GetInstance()->Initialize();

	//�Q�[���̏��
	GameStateManager::GetInstance()->Initialize();

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
	Player::GetInstance()->Update();
	StagePanel::GetInstance()->Update();
	GameMode::GetInstance()->Update();
	GameStateManager::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemyManager->Update();
	enemyManager->GetCameraData(camera->GetViewMatrix(), camera->GetProjectionMatrix(), camera->GetViewPort());
	//�G��|������V�[���ȍ~(��)
	if (enemyManager->BossDestroy()) {
		SceneManager::GetInstance()->PopScene();
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
	Player::GetInstance()->ActUIDraw();
	enemyManager->UIDraw();
	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
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
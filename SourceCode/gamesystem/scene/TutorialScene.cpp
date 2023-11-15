﻿#include "TutorialScene.h"
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include "SkillManager.h"
#include <TutorialTask.h>
#include <Helper.h>
#include "MobEnemy.h"
#include <SceneManager.h>
//��ԑJ��
/*state�̕��я��ɍ��킹��*/
void (TutorialScene::* TutorialScene::stateTable[])() = {
	&TutorialScene::IntroState,//
	&TutorialScene::MoveState,//
	&TutorialScene::GetState,//�X�L���Q�b�g
	&TutorialScene::AttackState,//�U��
	&TutorialScene::DamageState,//�_���[�W��^����
	&TutorialScene::TutorialEnd,//�I���
};
//������
void TutorialScene::Initialize(DirectXCommon* dxCommon)
{
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//�|�X�g�G�t�F�N�g
	PlayPostEffect = false;
	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();
	//�X�L��
	SkillManager::GetInstance()->Initialize();
	//�Q�[���̏��
	GameStateManager::GetInstance()->SetDxCommon(dxCommon);
	GameStateManager::GetInstance()->Initialize();
	//�X�e�[�W�̏�
	StagePanel::GetInstance()->LoadResource();

	//{
	//	auto player = GameObject::CreateObject<Player>();
	//	player->LoadResource();
	//	player->Initialize();
	//	player->InitState({ -4.0f,0.1f,2.0f });
	//	GameStateManager::GetInstance()->SetPlayer(player);
	//}

	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TUTORIAL_START);
	/*player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ -4.0f,0.1f,2.0f });
	player_->Initialize();
	player_->SetTitleFlag(true);*/
	//敵
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();
	//enemy->SetPlayer(player_.get());

	_nowstate = TUTORIAL_INTRO;


	TutorialTask::GetInstance()->SetChoiceSkill(false);
}
//�X�V
void TutorialScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();

	lightGroup->Update();
	
	//�e�N���X�X�V
	camerawork->Update(camera);
	if (!GameStateManager::GetInstance()->GetIsFinish()) {
		//Player::GetInstance()->Update();
	}
	lightGroup->Update();
	//player_->Update();
	StagePanel::GetInstance()->Update();
	GameStateManager::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();
	if (input->TriggerButton(input->BACK)) {
		m_End = true;
	}
	//�G��|������V�[���ȍ~(��)
	if (m_End) {
		_ChangeType = CHANGE_TITLE;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	//�Ղꂢ��[��HP�������Ȃ��Ă��J�ڂ���
	if (player->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
		SceneManager::GetInstance()->ChangeScene("MAP");
		SceneChanger::GetInstance()->SetChange(false);
	}

	//��Ԉڍs(state�ɍ��킹��)
	(this->*stateTable[static_cast<size_t>(_nowstate)])();
}

void TutorialScene::Draw(DirectXCommon* dxCommon) {
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
void TutorialScene::FrontDraw(DirectXCommon* dxCommon) {
	ParticleEmitter::GetInstance()->FlontDrawAll();
	GameStateManager::GetInstance()->ActUIDraw();
	enemy->UIDraw();
	//player_->UIDraw();
	text_->TestDraw(dxCommon);
	ParticleEmitter::GetInstance()->FlontDrawAll();
	GameStateManager::GetInstance()->ActUIDraw();

	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
void TutorialScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);

	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
	//player_->Draw(dxCommon);
	enemy->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void TutorialScene::ImGuiDraw() {
	/*ImGui::Begin("Tutorial");
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::Text("State:%d", _nowstate);
	ImGui::End();
	
	TutorialTask::GetInstance()->ImGuiDraw();*/
	//player_->ImGuiDraw();
	GameStateManager::GetInstance()->ImGuiDraw();
}

void TutorialScene::Finalize() {

}
//�ŏ��̌��
void TutorialScene::IntroState() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 150, 1)) {
		_nowstate = TUTORIAL_MOVE;
		m_Timer = {};
	
	}
}
//�ړ�
void TutorialScene::MoveState() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 50, 1)) {
		m_Timer = {};
		TutorialTask::GetInstance()->SetTutorialState(TASK_BIRTH_BEFORE);
	}

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTHSKIL) {
		text_->SetConversation(TextManager::TUTORIAL_GET);
		_nowstate = TUTORIAL_GETSKILL;
	}
}
//�X�L���Q�b�g
void TutorialScene::GetState() {

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_ATTACK) {
		text_->SetConversation(TextManager::TUTORIAL_EXPLAIN);
		_nowstate = TUTORIAL_ATTACK;
		m_Timer = {};
	}
}
//�U��
void TutorialScene::AttackState() {
	Helper::GetInstance()->CheckMin(m_Timer, 410, 1);

	if (m_Timer == 200) {
		text_->SetConversation(TextManager::TUTORIAL_MARK);
	}
	else if (m_Timer == 400) {
		text_->SetConversation(TextManager::TUTORIAL_TEXT_ATTACK);
	}

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_DAMAGE) {
		text_->SetConversation(TextManager::TUTORIAL_TEXT_DAMAGE);
		_nowstate = TUTORIAL_DAMAGE;
		m_Timer = {};
	}
}
//�_���[�W��������
void TutorialScene::DamageState() {
	if (enemy->GetHP() <= 0.0f) {
		m_Timer++;
		if (m_Timer == 1) {
			text_->SetConversation(TextManager::TUTORIAL_SKILL);
		}
		else if (m_Timer == 200) {
			text_->SetConversation(TextManager::TUTORIAL_CHOICE);
		}
		if (TutorialTask::GetInstance()->GetChoiceSkill()) {
			text_->SetConversation(TextManager::TUTORIAL_END);
			_nowstate = TUTORIAL_FINISH;
			m_Timer = {};
		}
		GameStateManager::GetInstance()->StageClearInit();
	}
}
//�`���[�g���A���I���
void TutorialScene::TutorialEnd() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 200, 1)) {
		m_Timer = {};
		m_End = true;
	}
}
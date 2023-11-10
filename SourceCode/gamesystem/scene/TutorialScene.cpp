#include "TutorialScene.h"
#include <Player.h>
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include "SkillManager.h"
#include <TutorialTask.h>
#include <Helper.h>
#include "BattleScene.h"
#include "TutorialEnemy.h"

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

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

	{
		auto player = GameObject::CreateObject<Player>();
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
	StagePanel::GetInstance()->Initialize();
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TUTORIAL_START);
	//�G
	enemy = make_unique<TutorialEnemy>();
	enemy->Initialize();

	_nowstate = TUTORIAL_INTRO;

	//���U���g�e�L�X�g
	resulttext = make_unique<TextManager>();
	resulttext->Initialize(dxCommon);
	resulttext->SetConversation(TextManager::RESULT, { 5.0f,280.0f });

	//�ۉe
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
}
//�X�V
void TutorialScene::Update(DirectXCommon* dxCommon)
{
	Input* input = Input::GetInstance();
	//�v���C���[
	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.5f, Player::GetInstance()->GetPosition().z }));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	//�{�X
	lightGroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemy->GetPosition().x, 	0.5f, 	enemy->GetPosition().z }));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));
	lightGroup->Update();
	// �S�I�u�W�F�N�g�X�V
	game_object_manager_->Update();

	//�e�N���X�X�V
	camerawork->Update(camera);
	lightGroup->Update();
	game_object_manager_->Update();
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
	if (GameStateManager::GetInstance()->GetPlayer().lock()->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
		SceneManager::GetInstance()->ChangeScene<BattleScene>();
		/*if (_ChangeType == CHANGE_TITLE) {
			SceneManager::GetInstance()->PopScene();
		}
		else {
			
		}*/
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
	////���S�ɑO�ɏ����X�v���C�g
	text_->TestDraw(dxCommon);
	if (_nowstate == TUTORIAL_DAMAGE) {
		resulttext->TestDraw(dxCommon);
	}
	ParticleEmitter::GetInstance()->FlontDrawAll();
	GameStateManager::GetInstance()->ActUIDraw();
	game_object_manager_->UIDraw();

// <<<<<<< HEAD
// 	enemyManager->UIDraw();
// =======
// 	//Player::GetInstance()->UIDraw();
// 	enemy->UIDraw();
// >>>>>>> 5735619e9defc9fdb26571e999c2bcb5a575bea5
	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
void TutorialScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	game_object_manager_->Draw();
	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemy->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui
void TutorialScene::ImGuiDraw() {
	ImGui::Begin("Tutorial");
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::Text("State:%d", _nowstate);
	ImGui::End();
	
	TutorialTask::GetInstance()->ImGuiDraw();
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
	if (Helper::GetInstance()->CheckMin(m_Timer, 150, 1)) {
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
		if (m_Timer == 10) {
			text_->SetConversation(TextManager::TUTORIAL_SKILL);
		}
		else if (m_Timer == 200) {
			text_->SetConversation(TextManager::TUTORIAL_END);
		}
		else if (m_Timer == 400) {
			_nowstate = TUTORIAL_FINISH;
		}
		GameStateManager::GetInstance()->StageClearInit();
	}
}
//�`���[�g���A���I���
void TutorialScene::TutorialEnd() {
	if (Helper::GetInstance()->CheckMin(m_Timer, 150, 1)) {
		m_Timer = {};
		m_End = true;
	}
}
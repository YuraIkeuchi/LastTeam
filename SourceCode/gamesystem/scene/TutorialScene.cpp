#include "TutorialScene.h"
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include <ImageManager.h>
#include <TutorialTask.h>
#include <Helper.h>
#include "MobEnemy.h"
#include <SceneManager.h>
#include <StageBack.h>
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
void TutorialScene::Initialize(DirectXCommon* dxCommon) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//�|�X�g�G�t�F�N�g
	PlayPostEffect = false;
	//�p�[�e�B�N���S�폜
	ParticleEmitter::GetInstance()->AllDelete();


	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TUTORIAL_START, m_TextPos);

	_nowstate = TUTORIAL_INTRO;

	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ -PANEL_SIZE * 2.f,0.1f,PANEL_SIZE });
	player_->Initialize();
	//ゲームステート初期化
	GameStateManager::GetInstance()->SetDxCommon(dxCommon);
	GameStateManager::SetPlayer(player_.get());
	GameStateManager::GetInstance()->Initialize();
	//ステージパネルの初期化
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	//ビヘイビア試しました！
	{
		//auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
	}

	skipUI = IKESprite::Create(ImageManager::TUTORIAL_SKIPUI, { 940.f ,5.f });
	skipUnder = IKESprite::Create(ImageManager::FEED, { 940.f + (128.f* scale_skip) ,5.f });
	skipBack = IKESprite::Create(ImageManager::FEED, { 940.f +  (128.f * scale_skip),5.f });
	skipUI->SetSize({ 472.f * scale_skip ,96.f * scale_skip });
	skipUnder->SetSize({ 0.f,96.f * scale_skip });
	skipUnder->SetColor({ 0,1,1,1 });
	skipBack->SetSize({ 344.f * scale_skip,96.f * scale_skip });

	window.sprite = IKESprite::Create(ImageManager::TUTORIAL_WINDOW, { 0.0f,0.0f });
	window.sprite->SetAnchorPoint({ 0.5f,0.5f });
	window.m_Pos = { 750.0f,630.0f };

	//敵
	InterEnemy::SetPlayer(player_.get());
	enemy = std::make_unique<MobEnemy>();
	enemy->Initialize();

	TutorialTask::GetInstance()->SetChoiceSkill(false);

	Feed* feed_ = new Feed();
	feed.reset(feed_);
}
//�X�V
void TutorialScene::Update(DirectXCommon* dxCommon) {
	if (m_IsBackKey) {
		const float frameMax = 5.f;
		if (Helper::FrameCheck(frame, 1 / frameMax)) {
			m_Skip = true;
			m_IsBackKey = false;
			_nowstate = TUTORIAL_DAMAGE;
		} else {
			float siz = 0.f;
			siz = Ease(In, Quad, frame, 0.f, 344.f);
			skipUnder->SetSize({ siz * scale_skip,96.f * scale_skip });
		}
		return;
	}
	Input* input = Input::GetInstance();

	lightGroup->Update();
	//�e�N���X�X�V
	camerawork->Update(camera);
	player_->Update();
	GameStateManager::GetInstance()->Update();

	StagePanel::GetInstance()->Update();
	if (!m_Skip) {
		enemy->Update();
	}
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	if (input->TriggerButton(input->BACK) &&
		!m_IsBackKey &&
		!m_Skip) {
		m_IsBackKey = true;
		m_Timer = {};
	}
	//�G��|������V�[���ȍ~(��)
	if (m_End) {
		_ChangeType = CHANGE_TITLE;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	//�Ղꂢ��[��HP�������Ȃ��Ă��J�ڂ���
	if (player_->GetHp() <= 0.0f) {
		_ChangeType = CHANGE_OVER;
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {
		TutorialTask::GetInstance()->SetViewSkill(true);
		player_->PlayerSave();
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
		SceneManager::GetInstance()->ChangeScene("MAP");
		SceneChanger::GetInstance()->SetChange(false);
	}

	//��Ԉڍs(state�ɍ��킹��)
	(this->*stateTable[static_cast<size_t>(_nowstate)])();

	//ウィンドウの拡大
	if (Helper::FrameCheck(window.m_Frame, 0.1f)) {
		window.m_Frame = 1.0f;
	}

	window.m_Size = { Ease(In,Cubic,window.m_Frame,0.0f,960.0f),
	Ease(In,Cubic,window.m_Frame,0.0f,164.0f), };
	window.sprite->SetSize(window.m_Size);
	window.sprite->SetPosition(window.m_Pos);
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
//�|�X�g�G�t�F�N�g������Ȃ�
void TutorialScene::FrontDraw(DirectXCommon* dxCommon) {
	if (!m_FeedEnd) {
		ParticleEmitter::GetInstance()->FlontDrawAll();

		GameStateManager::GetInstance()->ActUIDraw();
		enemy->UIDraw();
		player_->UIDraw();
	}
	if (!TutorialTask::GetInstance()->GetViewSkill()) {
		window.sprite->Draw();
		text_->TestDraw(dxCommon);
	}
	if (!m_FeedEnd) {
		IKESprite::PreDraw();
		skipBack->Draw();
		skipUnder->Draw();
		skipUI->Draw();
		IKESprite::PostDraw();
	}
	if (m_Feed) {
		feed->Draw();
	}
	SceneChanger::GetInstance()->Draw();
}
//�|�X�g�G�t�F�N�g������
void TutorialScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	StageBack::GetInstance()->Draw(dxCommon);
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
	if (enemy->GetHP() > 0.0f && !m_Skip) {
		player_->Draw(dxCommon);
		enemy->Draw(dxCommon);
		StagePanel::GetInstance()->ActDraw(dxCommon);
	}
}
//ImGui
void TutorialScene::ImGuiDraw() {
	ImGui::Begin("Tutorial");
	ImGui::Text("Timer:%d", m_Timer);
	ImGui::Text("State:%d", _nowstate);
	ImGui::End();
	/*
	TutorialTask::GetInstance()->ImGuiDraw();
	player_->ImGuiDraw();
	*/
	GameStateManager::GetInstance()->ImGuiDraw();
}

void TutorialScene::Finalize() {

}
//�ŏ��̌��
void TutorialScene::IntroState() {
	if (Helper::CheckMin(m_Timer, 150, 1)) {
		_nowstate = TUTORIAL_MOVE;
		m_Timer = {};

	}
}
//�ړ�
void TutorialScene::MoveState() {
	if (Helper::CheckMin(m_Timer, 50, 1)) {
		m_Timer = {};
		TutorialTask::GetInstance()->SetTutorialState(TASK_BIRTH_BEFORE);
	}

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTHSKIL) {
		text_->SetConversation(TextManager::TUTORIAL_GET, m_TextPos);
		_nowstate = TUTORIAL_GETSKILL;
	}
}
//�X�L���Q�b�g
void TutorialScene::GetState() {

	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_ATTACK) {
		text_->SetConversation(TextManager::TUTORIAL_EXPLAIN, m_TextPos);
		_nowstate = TUTORIAL_ATTACK;
		m_Timer = {};
	}
}
//�U��
void TutorialScene::AttackState() {
	Helper::CheckMin(m_Timer, 410, 1);

	if (m_Timer == 120) {
		text_->SetConversation(TextManager::TUTORIAL_MARK, m_TextPos);
	} else if (m_Timer == 240) {
		text_->SetConversation(TextManager::TUTORIAL_TEXT_ATTACK, m_TextPos);
	}
	if (TutorialTask::GetInstance()->GetTutorialState() == TASK_DAMAGE) {
		text_->SetConversation(TextManager::TUTORIAL_TEXT_DAMAGE, m_TextPos);
		_nowstate = TUTORIAL_DAMAGE;
		m_Timer = {};
	}
}
//�_���[�W��������
void TutorialScene::DamageState() {
	if (enemy->GetHP() <= 0.0f || m_Skip) {
		if (!m_FeedStart) {
			m_Feed = true;
			m_FeedStart = true;
		}
		if (m_Feed) {
			feed->FeedIn(Feed::FeedType::WHITE, 1.0f / 60.0f, m_Feed);
		}
		if (feed->GetFeedEnd()) {
			m_FeedEnd = true;
			m_Timer = {};
		}

		if (m_FeedEnd) {
			window.m_Pos = { 640.0f,550.0f };
			Audio::GetInstance()->StopWave(AUDIO_MAIN);
			m_Timer++;
			if (m_Timer == 1) {
				m_TextPos = { 120.0f,20.f };
				text_->SetConversation(TextManager::TUTORIAL_SKILL, m_TextPos);
			} else if (m_Timer == 120) {
				m_TextPos = { 10.0f,20.f };
				text_->SetConversation(TextManager::TUTORIAL_CHOICE, m_TextPos);
			} else if (m_Timer == 170) {
				TutorialTask::GetInstance()->SetViewSkill(true);
			}
			if (TutorialTask::GetInstance()->GetChoiceSkill()) {
				TutorialTask::GetInstance()->SetViewSkill(false);
				text_->SetConversation(TextManager::TUTORIAL_END, m_TextPos);
				_nowstate = TUTORIAL_FINISH;
				m_Timer = {};
			}
			GameStateManager::GetInstance()->StageClearInit();
		}
	}
}
//�`���[�g���A���I���
void TutorialScene::TutorialEnd() {
	if (Helper::CheckMin(m_Timer, 100, 1)) {
		m_Timer = {};
		m_End = true;
	}
}
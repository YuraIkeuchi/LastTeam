﻿#include "TutorialScene.h"
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <SceneChanger.h>
#include <ImageManager.h>
#include <TutorialTask.h>
#include <Helper.h>
#include "TutorialEnemy.h"
#include <SceneManager.h>
#include <StageBack.h>
//��ԑJ��
/*state�̕��я��ɍ��킹��*/
void (TutorialScene::* TutorialScene::stateTable[])() = {
	&TutorialScene::PlayState,//�_���[�W��^����
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

	m_TextPos = { -10.0f,-30.0f };
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon,LOAD_TUTORIAL);
	text_->SetConversation(TextManager::TUTORIAL_START, m_TextPos);

	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ -PANEL_SIZE * 2.f,0.1f,PANEL_SIZE });
	player_->Initialize();
	player_->SkipInitialize();
	player_->SetHp(500.0f);
	//ゲームステート初期化
	GameStateManager::GetInstance()->SetDxCommon(dxCommon);
	GameStateManager::SetPlayer(player_.get());
	GameStateManager::GetInstance()->Initialize();
	GameStateManager::GetInstance()->SetGameStart(true);

	TutorialTask::GetInstance()->Initialize();
	//ステージパネルの初期化
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	//ビヘイビア試しました！
	{
		//auto test_enemy_1 = GameObject::CreateObject<TestEnemy>();
	}

	skipUI = IKESprite::Create(ImageManager::RESULTSKIP, { 1250.f -102.4f ,5.f });
	skipUI->SetSize({ 512.f * 0.4f,128.f * 0.4f });
	skipUI->SetAnchorPoint({0.5f,0.f});
	window.sprite = IKESprite::Create(ImageManager::TUTORIAL_WINDOW, { 0.0f,0.0f });
	window.sprite->SetAnchorPoint({ 0.5f,0.5f });
	window.m_Pos = { 750.0f,630.0f };

	//敵
	InterEnemy::SetPlayer(player_.get());
	for (int i = 0; i < enemy.size(); i++) {
		enemy[i] = std::make_unique<TutorialEnemy>();
		enemy[i]->SetNumber(i);
		enemy[i]->Initialize();
	}

	TutorialTask::GetInstance()->SetChoiceSkill(false);

	Feed* feed_ = new Feed();
	feed.reset(feed_);
}
//�X�V
void TutorialScene::Update(DirectXCommon* dxCommon) {
	if (m_IsBackKey) {
		const float frameMax = 10.f;
		if (Helper::FrameCheck(frame, 1 / frameMax)) {
			m_Skip = true;
			TutorialTask::GetInstance()->AllFinish();
			m_IsBackKey = false;
		} else {
			float siz = Ease(In, Back, frame, 512.f * 0.4f, 0.f);
			skipUI->SetSize({ siz , 128.f * 0.4f });
		}
		return;
	}
	Input* input = Input::GetInstance();

	if (!m_FeedEnd) {
		m_TextTimer++;
	}

	Helper::Clamp(m_TextTimer, 0, 600);

	if (m_TextTimer == 150) {
		text_->SetConversation(TextManager::TUTORIAL_TASK, m_TextPos);
	}
	else if (m_TextTimer == 350) {
		text_->SetConversation(TextManager::TUTORIAL_COUNTER, m_TextPos);
	}
	else if (m_TextTimer == 550) {
		text_->SetConversation(TextManager::TUTORIAL_COUNTER2, m_TextPos);
	}

	if (!m_Counter && TutorialTask::GetInstance()->GetTaskFinish(TASK_COUNTER)) {
		m_CounterEffect = true;
		text_->SetConversation(TextManager::TUTORIAL_SHOT, m_TextPos);
		m_Counter = true;
	}

	//最初の一回はカウンターエフェクトを見せるためテキストを消す
	if (m_CounterEffect) {
		if (Helper::CheckMin(m_CounterTimer, 60, 1)) {
			m_CounterEffect = false;
		}
	}
	lightGroup->Update();
	//�e�N���X�X�V
	camerawork->Update(camera);
	player_->Update();
	GameStateManager::GetInstance()->Update();

	StagePanel::GetInstance()->Update();
	if (!m_Skip) {
		for (int i = 0; i < enemy.size(); i++) {
			enemy[i]->Update();
		}
	}
	ParticleEmitter::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	if ((input->TriggerButton(Input::BACK)||
		input->TriggerKey(DIK_BACK)) &&
		!m_IsBackKey &&
		!m_Skip &&
		!m_EnemyDelete) {
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
		s_PlayBGM = true;
		TutorialTask::GetInstance()->SetViewSkill(true);
		player_->PlayerSave();
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

	TutorialTask::GetInstance()->Update();
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
		for (int i = 0; i < enemy.size(); i++) {
			enemy[i]->UIDraw();
		}
		GameStateManager::GetInstance()->ActUIDraw();
		
		player_->UIDraw();
		TutorialTask::GetInstance()->Draw();
	}
	
	if (!m_FeedEnd || GameStateManager::GetInstance()->GetEndResult() && m_Timer <= 500) {
		if (!m_CounterEffect) {
			window.sprite->Draw();
			text_->TestDraw(dxCommon);
		}
	}
	if (!m_FeedEnd) {
		IKESprite::PreDraw();
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
	if (!m_EnemyDelete && !m_Skip) {
		player_->Draw(dxCommon);
		for (int i = 0; i < enemy.size(); i++) {
			enemy[i]->Draw(dxCommon);
		}
		StagePanel::GetInstance()->ActDraw(dxCommon);
	}
}
//ImGui
void TutorialScene::ImGuiDraw() {
	/*GameStateManager::GetInstance()->ImGuiDraw();
	ImGui::Begin("Tutorial");
	ImGui::Text("COunte:%d", m_CounterEffect);
	ImGui::Text("Timer:%d", m_CounterTimer);
	ImGui::Text("Counter:%d", m_Timer);
	ImGui::End();*/
}

void TutorialScene::Finalize() {

}

void TutorialScene::PlayState() {
	Input* input = Input::GetInstance();
	if (enemy[0]->GetHP() <= 0.0f && enemy[1]->GetHP() <= 0.0f) {
		m_EnemyDelete = true;
	}
	if (m_EnemyDelete || m_Skip) {

		TutorialTask::GetInstance()->SetTaskFinish(true, TASK_BREAK);
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
			if (GameStateManager::GetInstance()->GetEndResult()) {
				window.m_Pos = { 740.0f,550.0f };
				Audio::GetInstance()->StopWave(AUDIO_MAIN);
				Helper::CheckMin(m_Timer, 601, 1);
				if (TutorialTask::GetInstance()->GetChoiceSkill()) {
					TutorialTask::GetInstance()->SetViewSkill(false);
					text_->SetConversation(TextManager::TUTORIAL_END, m_TextPos);
					_nowstate = TUTORIAL_FINISH;
					m_Timer = {};
				}
			}
			if (m_Timer == 1) {
				m_TextPos = { 20.0f,20.f };
				text_->SetConversation(TextManager::TUTORIAL_SKILL, m_TextPos);
			}
			else if (m_Timer == 170) {
				m_TextPos = { 20.0f,20.f };
				text_->SetConversation(TextManager::TUTORIAL_DELETE, m_TextPos);
			}
			else if (m_Timer == 340) {
				m_TextPos = { 20.0f,20.f };
				text_->SetConversation(TextManager::TUTORIAL_CHOICE, m_TextPos);
			}
			else if (m_Timer == 500) {
				GameStateManager::GetInstance()->SetEndText(true);
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
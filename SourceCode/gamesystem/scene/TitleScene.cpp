#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <Player.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include "MobEnemy.h"
#include "TutorialTask.h"
#include <GameStateManager.h>
// �J�ڂ�����V�[��
#include "BattleScene.h"
#include "MapScene.h"
#include "TutorialScene.h"
#include "TextManager.h"
//������
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TITLE);
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}

	//�v���C���[
	Player::GetInstance()->LoadResource();

	//�X�e�[�W�̏�
	StagePanel::GetInstance()->LoadResource();
	
	GameReset({ -4.0f,0.1f,2.0f });

	//�G
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();

	//�^�C�g��
	title_[TITLE_BACK] = IKESprite::Create(ImageManager::TITLEBACK, { 0.0f,0.0f });
	title_[TITLE_TEXT] = IKESprite::Create(ImageManager::TITLETEXT, { 640.0f,200.0f },{1.0f,1.0f,1.0f,1.0f},{0.5f,0.5f});

}
//�X�V
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera);
	// �S�I�u�W�F�N�g�X�V
	game_object_manager_->Update();

	//�e�N���X�X�V
	camerawork->Update(camera);
	lightGroup->Update();
	
	//�e�N���X�X�V
	Player::GetInstance()->TitleUpdate();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();

	if ((input->TriggerButton(input->B))) {			//�o�g��
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
		//�`���[�g���A���̃^�X�N
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);

	}
	if (input->TriggerKey(DIK_SPACE)) {			//�}�b�v
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = MAP;
		//�`���[�g���A���̃^�X�N
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
	}
	if (input->TriggerButton(input->X)) {			//�`���[�g���A��
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = TUTORIAL;
		//�`���[�g���A���̃^�X�N
		TutorialTask::GetInstance()->SetTutorialState(TASK_MOVE);
	}
	if (SceneChanger::GetInstance()->GetChange()) {			//�^���ÂɂȂ�����ς��
		if (_SceneType == PLAY) {
			GameReset({ -8.0f,0.1f,0.0f });
			SceneManager::GetInstance()->ChangeScene<BattleScene>();
		} else if(_SceneType == MAP) {
			SceneManager::GetInstance()->ChangeScene<MapScene>();
		}
		else {
			GameReset({ -4.0f, 0.1f, 2.0f });
			SceneManager::GetInstance()->ChangeScene<TutorialScene>();
		}
		SceneChanger::GetInstance()->SetChange(false);
	}

}
//�`��
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//�O�ʕ`��
void TitleScene::FrontDraw(DirectXCommon* dxCommon) {
	////���S�ɑO�ɏ����X�v���C�g
	//for (int i = 0; i < 3; i++) {
	//	if (i != 0) {
	//		if (texts[(size_t)i - 1]->GetFinish()) {
	//			texts[i]->Draw();

	//		}
	//	} else {
	//		texts[i]->Draw();
	//	}
	//}
	//Font::PostDraw();
	text_->TestDraw(dxCommon);
	IKESprite::PreDraw();
	title_[TITLE_TEXT]->Draw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();

}
//�w�ʕ`��
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	title_[TITLE_BACK]->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemy->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui�`��
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	camerawork->ImGuiDraw();
}
//���
void TitleScene::Finalize() {
}

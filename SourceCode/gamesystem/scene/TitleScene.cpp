#include "TitleScene.h"
#include "SceneManager.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"

// �J�ڂ�����V�[��
#include "BattleScene.h"
#include "MapScene.h"

//������
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	text_ = make_unique<TextManager>();
	text_->Initialize();
	text_->SetConversation(TextManager::FIRST);
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}
}
//�X�V
void TitleScene::Update(DirectXCommon* dxCommon) {
	camerawork->Update(camera);
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {			//�o�g��
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
	
	}
	if (input->TriggerKey(DIK_SPACE)) {			//�}�b�v
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = MAP;
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//�^���ÂɂȂ�����ς��
		if (_SceneType == PLAY) {
			SceneManager::GetInstance()->ChangeScene<BattleScene>();
		}
		else {
			SceneManager::GetInstance()->ChangeScene<MapScene>();
		}
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
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
	}
	else {
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
	//���S�ɑO�ɏ����X�v���C�g
	IKESprite::PreDraw();
	text_->FlowDraw();
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();

}
//�w�ʕ`��
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui�`��
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("TITLE");
	ImGui::Text("Title");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
}
//���
void TitleScene::Finalize() {
}

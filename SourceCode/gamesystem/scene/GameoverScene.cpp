#include "GameoverScene.h"
#include "ImageManager.h"
#include "SceneManager.h"

// �J�ڂ�����V�[��
#include "TitleScene.h"
#include "MapScene.h"
//������
void GameoverScene::Initialize(DirectXCommon* dxCommon) {

	//���ʂ̏�����
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
}
//�X�V
void GameoverScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {			//�o�g��
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//�^���ÂɂȂ�����ς��
		SceneManager::GetInstance()->ChangeScene<TitleScene>(true);
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
}
//�`��
void GameoverScene::Draw(DirectXCommon* dxCommon) {
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
void GameoverScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	sprite->Draw();
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();
}
//�w�ʕ`��
void GameoverScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui�`��
void GameoverScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Gameover");
	ImGui::Text("Gameover");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
}
//���
void GameoverScene::Finalize() {
}
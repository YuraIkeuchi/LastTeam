#include "ClearScene.h"
#include "ImageManager.h"
#include "SceneManager.h"

// �J�ڂ�����V�[��
#include "TitleScene.h"
#include "MapScene.h"
//������
void ClearScene::Initialize(DirectXCommon* dxCommon) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::GAMECLEARBACK, { 0.0f,0.0f });
}
//�X�V
void ClearScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {			//�o�g��
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//�^���ÂɂȂ�����ς��
		GameReset({ -4.0f,0.1f,2.0f });
		SceneManager::GetInstance()->ChangeScene<TitleScene>(true);
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
}
//�`��
void ClearScene::Draw(DirectXCommon* dxCommon) {
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
void ClearScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	sprite->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();
}
//�w�ʕ`��
void ClearScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui�`��
void ClearScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Clear");
	ImGui::Text("Clear");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
}
//���
void ClearScene::Finalize() {
}
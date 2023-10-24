#include "TitleScene.h"
#include "SceneManager.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"

// �J�ڂ�����V�[��
#include "BattleScene.h"
#include "MapScene.h"
#include "TextManager.h"
//������
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	wchar_t* sample = TextManager::GetInstance()->SearchText(TextManager::TITLE_01);
	texts[0] = (std::move(std::make_unique<Font>(sample, XMFLOAT2{ 300.f,380.f }, XMVECTOR{ 1.f,1.f,1.f,1.f })));

	wchar_t* sample2 = TextManager::GetInstance()->SearchText(TextManager::TITLE_02);
	texts[1] = (std::move(std::make_unique<Font>(sample2, XMFLOAT2{ 300.f,420.f }, XMVECTOR{ 1.f,0.f,1.f,1.f })));

	wchar_t* sample3 = TextManager::GetInstance()->SearchText(TextManager::TITLE_03);
	texts[2] = (std::move(std::make_unique<Font>(sample3, XMFLOAT2{ 300.f,460.f }, XMVECTOR{ 1.f,0.f,0.f,1.f })));

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
		} else {
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
	//���S�ɑO�ɏ����X�v���C�g
	for (int i = 0; i < 3; i++) {
		if (i != 0) {
			if (texts[i - 1]->GetFinish()) {
				texts[i]->Draw();

			}
		} else {
			texts[i]->Draw();
		}
	}
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

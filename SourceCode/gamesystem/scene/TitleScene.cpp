#include "TitleScene.h"
#include "SceneManager.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"

// 遷移しうるシーン
#include "BattleScene.h"
#include "MapScene.h"
#include "TextManager.h"
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
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
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	camerawork->Update(camera);
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;

	}
	if (input->TriggerKey(DIK_SPACE)) {			//マップ
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = MAP;
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		if (_SceneType == PLAY) {
			SceneManager::GetInstance()->ChangeScene<BattleScene>();
		} else {
			SceneManager::GetInstance()->ChangeScene<MapScene>();
		}
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
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
//前面描画
void TitleScene::FrontDraw(DirectXCommon* dxCommon) {
	//完全に前に書くスプライト
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
//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("TITLE");
	ImGui::Text("Title");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
}
//解放
void TitleScene::Finalize() {
}

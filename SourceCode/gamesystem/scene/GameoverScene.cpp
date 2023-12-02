#include "GameoverScene.h"
#include "ImageManager.h"
#include <SceneManager.h>
#include <SceneManager.h>
//初期化
void GameoverScene::Initialize(DirectXCommon* dxCommon) {

	//共通の初期化
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::GAMEOVERBACK, { 0.0f,0.0f });
	Audio::GetInstance()->StopWave(AUDIO_MAIN);
}
//更新
void GameoverScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B)|| input->TriggerKey(DIK_SPACE))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
}
//描画
void GameoverScene::Draw(DirectXCommon* dxCommon) {
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
//前面描画
void GameoverScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	sprite->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();
}
//背面描画
void GameoverScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui描画
void GameoverScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Gameover");
	ImGui::Text("Gameover");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
}
//解放
void GameoverScene::Finalize() {
}
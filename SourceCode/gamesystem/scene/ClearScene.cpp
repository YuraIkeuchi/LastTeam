#include "ClearScene.h"
#include "ImageManager.h"
#include "SceneManager.h"

// 遷移しうるシーン
#include "TitleScene.h"
#include "MapScene.h"
//初期化
void ClearScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::GAMECLEARBACK, { 0.0f,0.0f });
}
//更新
void ClearScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		GameReset({ -4.0f,0.1f,2.0f });
		SceneManager::GetInstance()->ChangeScene<TitleScene>(true);
		SceneChanger::GetInstance()->SetChange(false);
	}

	SceneChanger::GetInstance()->Update();
}
//描画
void ClearScene::Draw(DirectXCommon* dxCommon) {
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
void ClearScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	sprite->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();
}
//背面描画
void ClearScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui描画
void ClearScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Clear");
	ImGui::Text("Clear");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
}
//解放
void ClearScene::Finalize() {
}
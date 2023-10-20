#include "TitleScene.h"
#include "SceneManager.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"

// 遷移しうるシーン
#include "BattleScene.h"
#include "MapScene.h"

//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	text_ = make_unique<TitleText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::FIRST);
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	camerawork->Update(camera);
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B))) {
		SceneManager::GetInstance()->ChangeScene<BattleScene>();
	}
	if (input->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene<MapScene>();
	}
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
void TitleScene::FrontDraw(DirectXCommon* dxCommon) {
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	text_->SpriteDraw(dxCommon);
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
}
//解放
void TitleScene::Finalize() {
}

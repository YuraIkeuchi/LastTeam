#include "TutorialSceneActor.h"
#include "ImageManager.h"
#include "Helper.h"

//初期化
void TutorialSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	Audio::GetInstance()->StopWave(AUDIO_TITLE);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_LOAD, VolumManager::GetInstance()->GetBGMVolum() + 0.5f);
	//ポストエフェクト
	PlayPostEffect = false;
	//カメラ更新
	camerawork->Update(camera);

	lightgroup->SetCircleShadowActive(0, false);
	lightgroup->SetCircleShadowActive(1, false);
}
//更新
void TutorialSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	lightgroup->Update();
}
//描画
void TutorialSceneActor::Draw(DirectXCommon* dxCommon) {
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		postEffect->ImGuiDraw();
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
//解放
void TutorialSceneActor::Finalize() {
}
//後ろの描画
void TutorialSceneActor::BackDraw(DirectXCommon* dxCommon) {

}
//ポストエフェクトがかからない
void TutorialSceneActor::FrontDraw(DirectXCommon* dxCommon) {

}
//IMGuiの描画
void TutorialSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {

}

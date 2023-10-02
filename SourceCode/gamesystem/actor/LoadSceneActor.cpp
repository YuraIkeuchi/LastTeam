#include "LoadSceneActor.h"
#include "ImageManager.h"
#include "Easing.h"
#include "Helper.h"
//初期化
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

}
//更新
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	lightgroup->Update();
	camerawork->Update(camera);
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
}
//描画
void LoadSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		SpriteDraw();
		//ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		//ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		SpriteDraw();
		dxCommon->PostDraw();
	}
}
//前面描画
void LoadSceneActor::SpriteDraw() {
}
//ロード中の動き
void LoadSceneActor::IntroUpdate(DebugCamera* camera) {
}
void LoadSceneActor::MainUpdate(DebugCamera* camera) {
}
void LoadSceneActor::FinishUpdate(DebugCamera* camera) {

}
//背面描画
void LoadSceneActor::BackDraw(DirectXCommon* dxCommon) {

}
//ImGuiの描画
void LoadSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {

}
//解放
void LoadSceneActor::Finalize() {
}
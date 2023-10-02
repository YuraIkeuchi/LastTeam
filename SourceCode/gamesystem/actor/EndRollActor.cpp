#include "EndRollActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "Audio.h"
#include "Helper.h"
//初期化
void EndRollActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//このシーンだけセピアカラーつかう
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");
	Audio::GetInstance()->LoopWave(AUDIO_TITLE, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//シーンチェンジャー
	PlayPostEffect = false;
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();
	m_SceneState = SceneState::IntroState;
}
//更新
void EndRollActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
}
//描画
void EndRollActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		//ImGuiDraw(dxCommon);
		FrontDraw();
		//postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		dxCommon->PreDraw();
		//ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//前面描画
void EndRollActor::FrontDraw() {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
}
void EndRollActor::IntroUpdate(DebugCamera* camera) {
}
void EndRollActor::MainUpdate(DebugCamera* camera) {
}
void EndRollActor::FinishUpdate(DebugCamera* camera) {
}

//背面
void EndRollActor::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui描画
void EndRollActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void EndRollActor::Finalize() {
}

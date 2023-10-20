//#include "GameOverSceneActor.h"
//#include "SceneManager.h"
//#include <Easing.h>
//#include "ImageManager.h"
//#include "VariableCommon.h"
//#include "Audio.h"
////初期化
//void GameOverSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
//	//共通の初期化
//	BaseInitialize(dxCommon);
//	//オーディオ
//	Audio::GetInstance()->PlayWave("Resources/Sound/SE/gameOverSE.wav", VolumManager::GetInstance()->GetSEVolum());
//	//シーンチェンジャー
//	PlayPostEffect = false;
//	sceneChanger_ = make_unique<SceneChanger>();
//	sceneChanger_->Initialize();
//	m_SceneState = SceneState::IntroState;
//}
////更新
//void GameOverSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
//	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
//}
////描画
//void GameOverSceneActor::Draw(DirectXCommon* dxCommon) {
//	//ポストエフェクトをかけるか
//	if (PlayPostEffect) {
//		dxCommon->PreDraw();
//		//postEffect->Draw(dxCommon->GetCmdList());
//		BackDraw(dxCommon);
//		FrontDraw();
//		ImGuiDraw(dxCommon);
//		dxCommon->PostDraw();
//	} else {
//		dxCommon->PreDraw();
//		ImGuiDraw(dxCommon);
//		BackDraw(dxCommon);
//		FrontDraw();
//		dxCommon->PostDraw();
//	}
//}
////前面描画
//void GameOverSceneActor::FrontDraw() {
//}
//void GameOverSceneActor::IntroUpdate(DebugCamera* camera) {
//
//}
//void GameOverSceneActor::MainUpdate(DebugCamera* camera) {
//
//}
//void GameOverSceneActor::FinishUpdate(DebugCamera* camera) {
//}
////背面
//void GameOverSceneActor::BackDraw(DirectXCommon* dxCommon) {
//
//}
////ImGui描画
//void GameOverSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
//	//SceneSave::GetInstance()->ImGuiDraw();
//}
////解放
//void GameOverSceneActor::Finalize() {
//}

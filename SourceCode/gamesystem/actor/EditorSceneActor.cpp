//#include "EditorSceneActor.h"
//#include "Audio.h"
//#include "SceneManager.h"
//#include "ModelManager.h"
//#include "VariableCommon.h"
//#include "ParticleEmitter.h"
//
////初期化
//void EditorSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
//{
//	dxCommon->SetFullScreen(true);
//	//共通の初期化
//	BaseInitialize(dxCommon);
//
//	//ポストエフェクトのファイル指定
//	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");
//
//	ParticleEmitter::GetInstance()->AllDelete();
//}
////更新
//void EditorSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
//{
//	lightgroup->Update();
//	camerawork->Update(camera);
//
//	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
//}
////描画
//void EditorSceneActor::Draw(DirectXCommon* dxCommon)
//{
//	//描画方法
//	//ポストエフェクトをかけるか
//	if (PlayPostEffect) {
//		postEffect->PreDrawScene(dxCommon->GetCmdList());
//		BackDraw(dxCommon);
//		postEffect->PostDrawScene(dxCommon->GetCmdList());
//		dxCommon->PreDraw();
//		postEffect->Draw(dxCommon->GetCmdList());
//		FrontDraw(dxCommon);
//		ImGuiDraw(dxCommon);
//		postEffect->ImGuiDraw();
//		dxCommon->PostDraw();
//	}
//	else {
//		postEffect->PreDrawScene(dxCommon->GetCmdList());
//		postEffect->Draw(dxCommon->GetCmdList());
//		postEffect->PostDrawScene(dxCommon->GetCmdList());
//		dxCommon->PreDraw();
//		ImGuiDraw(dxCommon);
//
//		BackDraw(dxCommon);
//		FrontDraw(dxCommon);
//		dxCommon->PostDraw();
//	}
//}
////解放
//void EditorSceneActor::Finalize()
//{
//}
////後ろの描画
//void EditorSceneActor::BackDraw(DirectXCommon* dxCommon)
//{
//	IKEObject3d::PreDraw();
//	IKEObject3d::PostDraw();
//}
////ポストエフェクトがかからない
//void EditorSceneActor::FrontDraw(DirectXCommon* dxCommon) {
//	IKESprite::PreDraw();
//	//load2->Draw();..
//	IKESprite::PostDraw();
//#pragma endregion
//}
////IMGuiの描画
//void EditorSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
//}
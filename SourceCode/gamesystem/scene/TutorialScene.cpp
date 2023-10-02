#include "TutorialScene.h"
//初期化
void TutorialScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//タイトルのアクター
	BaseActor* actor_;
	actor_ = new TutorialSceneActor();
	actor_->Initialize(dxCommon, camera, lightGroup);
	actor.reset(actor_);
}
//更新
void TutorialScene::Update(DirectXCommon* dxCommon) {
	lightGroup->Update();
	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void TutorialScene::Draw(DirectXCommon* dxCommon) {
	actor->Draw(dxCommon);
}
//解放
void TutorialScene::Finalize() {
	actor->Finalize();
}
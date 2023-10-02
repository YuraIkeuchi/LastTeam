#include "TitleScene.h"
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//タイトルのアクター
	BaseActor* actor_;
	actor_ = new TitleSceneActor();
	actor_->Initialize(dxCommon, camera, lightGroup);
	actor.reset(actor_);
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	lightGroup->Update();
	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	actor->Draw(dxCommon);
}
//解放
void TitleScene::Finalize() {
	actor->Finalize();
}
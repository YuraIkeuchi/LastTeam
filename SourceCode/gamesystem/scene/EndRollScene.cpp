#include "EndRollScene.h"

//初期化
void EndRollScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//ロードのアクター
	BaseActor* actor_;
	actor_ = new EndRollActor();
	actor_->Initialize(dxCommon, camera, lightGroup);
	actor.reset(actor_);
}
//更新
void EndRollScene::Update(DirectXCommon* dxCommon) {
	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void EndRollScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	actor->Draw(dxCommon);
}
//解放
void EndRollScene::Finalize() {
	actor->Finalize();
}
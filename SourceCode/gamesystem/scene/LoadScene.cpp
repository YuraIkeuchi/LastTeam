#include "LoadScene.h"

//初期化
void LoadScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//ロードのアクター
	BaseActor* actor_;
	actor_ = new LoadSceneActor();
	actor_->Initialize(dxCommon,camera,lightGroup);
	actor.reset(actor_);
}
//更新
void LoadScene::Update(DirectXCommon* dxCommon) {
	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void LoadScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	actor->Draw(dxCommon);
}
//解放
void LoadScene::Finalize() {
	actor->Finalize();
}
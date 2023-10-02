#include "GameoverScene.h"
#include "GameOverSceneActor.h"
//初期化
void GameoverScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	//クリアのアクター
	BaseActor* actor_;
	actor_ = new GameOverSceneActor();
	actor_->Initialize(dxCommon, camera, lightGroup);
	actor.reset(actor_);
}
//更新
void GameoverScene::Update(DirectXCommon* dxCommon) {
	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void GameoverScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	actor->Draw(dxCommon);
}
//解放
void GameoverScene::Finalize() {
	actor->Finalize();
}
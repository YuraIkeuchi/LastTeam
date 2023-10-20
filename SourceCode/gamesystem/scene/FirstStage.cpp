#include "FirstStage.h"
//初期化
void FirstStage::Initialize(DirectXCommon* dxCommon)
{
	//共通の初期化
	BaseInitialize(dxCommon);
	//プレイシーンのアクター
	BaseActor* actor_;
	actor_ = new FirstStageActor();
	actor_->Initialize(dxCommon,camera,lightGroup);
	actor.reset(actor_);


	// 全オブジェクト初期化
	game_object_manager_->Initialize();
}
//更新
void FirstStage::Update(DirectXCommon* dxCommon)
{
	// 全オブジェクト更新
	game_object_manager_->Update();

	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void FirstStage::Draw(DirectXCommon* dxCommon)
{
	// 全オブジェクト描画
	game_object_manager_->Draw();
	actor->Draw(dxCommon);
}
//解放
void FirstStage::Finalize()
{
	// 全オブジェクト終了
	game_object_manager_->Finalize();
	actor->Finalize();
}
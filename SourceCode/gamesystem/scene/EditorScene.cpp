#include "EditorScene.h"
//初期化
void EditorScene::Initialize(DirectXCommon* dxCommon)
{
	dxCommon->SetFullScreen(false);
	//共通の初期化
	BaseInitialize(dxCommon);
	//エディターシーンのアクター
	BaseActor* actor_;
	actor_ = new EditorSceneActor();
	actor_->Initialize(dxCommon,camera,lightGroup);
	actor.reset(actor_);
}
//更新
void EditorScene::Update(DirectXCommon* dxCommon)
{
	actor->Update(dxCommon, camera, lightGroup);
}
//描画
void EditorScene::Draw(DirectXCommon* dxCommon)
{
	actor->Draw(dxCommon);
}
//解放
void EditorScene::Finalize()
{
	actor->Finalize();
}
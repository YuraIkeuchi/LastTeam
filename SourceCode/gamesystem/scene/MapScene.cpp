#include "MapScene.h"

void MapScene::Initialize(DirectXCommon* dxCommon) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//�v���C�V�[���̃A�N�^�[
	BaseActor* actor_;
	actor_ = new MapSceneActor();
	actor_->Initialize(dxCommon, camera, lightGroup);
	actor.reset(actor_);

}

void MapScene::Update(DirectXCommon* dxCommon) {
	actor->Update(dxCommon, camera, lightGroup);

}

void MapScene::Draw(DirectXCommon* dxCommon) {
	actor->Draw(dxCommon);
}

void MapScene::Finalize() {
	actor->Finalize();
}

#include "FirstStage.h"
//������
void FirstStage::Initialize(DirectXCommon* dxCommon)
{
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//�v���C�V�[���̃A�N�^�[
	BaseActor* actor_;
	actor_ = new FirstStageActor();
	actor_->Initialize(dxCommon,camera,lightGroup);
	actor.reset(actor_);


	// �S�I�u�W�F�N�g������
	game_object_manager_->Initialize();
}
//�X�V
void FirstStage::Update(DirectXCommon* dxCommon)
{
	// �S�I�u�W�F�N�g�X�V
	game_object_manager_->Update();

	actor->Update(dxCommon, camera, lightGroup);
}
//�`��
void FirstStage::Draw(DirectXCommon* dxCommon)
{
	// �S�I�u�W�F�N�g�`��
	game_object_manager_->Draw();
	actor->Draw(dxCommon);
}
//���
void FirstStage::Finalize()
{
	// �S�I�u�W�F�N�g�I��
	game_object_manager_->Finalize();
	actor->Finalize();
}
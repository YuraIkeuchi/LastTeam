#include "GuardAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"

//�ǂݍ���
GuardAction::GuardAction() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	//m_Object->SetScale({ 2.f,0.1f,2.f });
}
//������
bool GuardAction::Initialize() {
	m_Position = {};
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Color = { 0.3f,0.0f,1.0f,1.0f };
	m_Alive = true;
	m_Tag = "Guard";
	return true;
}
//�s��
void GuardAction::Action() {
	Obj_SetParam();
	Collide();
}
//�`��
void GuardAction::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//�G�t�F�N�g�`��
void GuardAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void GuardAction::ImGui_Origin() {
	ImGui::Begin("Guard");
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("PosZ:%f", m_Position.z);
	ImGui::End();
}
//�|�[�Y
void GuardAction::Pause() {

}
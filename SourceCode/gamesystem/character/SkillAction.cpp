#include "SkillAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"

//�ǂݍ���
SkillAction::SkillAction() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	//m_Object->SetScale({ 2.f,0.1f,2.f });
}
//������
bool SkillAction::Initialize() {
	m_Position = {};
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.3f,0.0f,1.0f,1.0f };
	m_Alive = true;
	m_Tag = "Skill";
	return true;
}
//�s��
void SkillAction::Action() {
	Obj_SetParam();
	Collide();
}
//�`��
void SkillAction::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//�G�t�F�N�g�`��
void SkillAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void SkillAction::ImGui_Origin() {

}
//�|�[�Y
void SkillAction::Pause() {

}
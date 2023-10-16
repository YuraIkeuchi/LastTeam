#include "AttackArea.h"
#include <StagePanel.h>
#include <ModelManager.h>
#include <Helper.h>
//�ǂݍ���
AttackArea::AttackArea() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::PANEL);
}
//������
bool AttackArea::Initialize() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	return true;
}

//�X�e�[�^�X������
void AttackArea::InitState(const int width, const int height) {
	m_Position = SetPanelPos(width, height);
	m_Scale = { 2.0f,0.1f,2.0f };
	m_Position.y = 0.1f;
	m_Color = { 1.0f,0.3f,0.0f,1.0f };
	m_Alive = true;
	m_AliveTimer = {};
}

//�X�V
void AttackArea::Update() {
	const int l_TargetTimer = 20;

	if (Helper::GetInstance()->CheckMin(m_AliveTimer, l_TargetTimer, 1)) {
		m_Alive = false;
	}

	Obj_SetParam();
}
//�`��
void AttackArea::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void AttackArea::ImGuiDraw() {
	ImGui::Begin("Area");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::End();
}
//�p�l���̈ʒu�ɒu��
XMFLOAT3 AttackArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
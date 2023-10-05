#include "InterAction.h"
#include "Player.h"
#include "Collision.h"
#include "StagePanel.h"
//�X�V
void InterAction::Update() {
	//�e�s��
	Action();
}

//�`��
void InterAction::Draw(DirectXCommon* dxCommon) {
}
//ImGui�`��
void InterAction::ImGuiDraw() {
	if (!this) { return; }
	ImGui_Origin();
}
//�v���C���[�ƃp�l���̓����蔻��
void InterAction::Collide()
{
	//���b�V�������肠��
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_Radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, m_Radius) &&
		(m_Alive))
	{
		//�v���C���[�̍s�����𑝂₵�p�l����߂�
		Player::GetInstance()->AddAct(m_Tag);
		StagePanel::GetInstance()->DeletePanel();
		m_Alive = false;
	}
}
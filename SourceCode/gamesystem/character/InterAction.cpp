#include "InterAction.h"
#include "Player.h"
#include "Collision.h"
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
//�v���C���[�ƃ{�X�̓����蔻��
void InterAction::Collide()
{
	//���b�V�������肠��
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_Radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, m_Radius) &&
		(m_Alive))
	{
		Player::GetInstance()->AddAct(m_Tag);
		m_Alive = false;
	}
}
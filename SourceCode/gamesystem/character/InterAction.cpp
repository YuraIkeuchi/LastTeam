#include "InterAction.h"
#include "Player.h"
#include "Collision.h"
//更新
void InterAction::Update() {
	//各行動
	Action();
}

//描画
void InterAction::Draw(DirectXCommon* dxCommon) {
}
//ImGui描画
void InterAction::ImGuiDraw() {
	if (!this) { return; }
	ImGui_Origin();
}
//プレイヤーとボスの当たり判定
void InterAction::Collide()
{
	//ラッシュ中判定あり
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_Radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, m_Radius) &&
		(m_Alive))
	{
		Player::GetInstance()->AddAct(m_Tag);
		m_Alive = false;
	}
}
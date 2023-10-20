#include "InterAction.h"
#include "Player.h"
#include "Collision.h"
#include "StagePanel.h"
#include <Easing.h>
#include "Helper.h"

void (InterAction::* InterAction::stateTable[])() = {
	&InterAction::Spawn,//動きの合間
	&InterAction::Alive,//動きの合間
	&InterAction::Vanish,//動きの合間
};

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
//プレイヤーとパネルの当たり判定
void InterAction::Collide()
{
	//ラッシュ中判定あり
	if (Collision::CircleCollision(m_Position.x, m_Position.z, m_Radius, Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().z, m_Radius) &&
		(m_Alive)){
		//プレイヤーの行動数を増やしパネルを戻す
		Player::GetInstance()->AddAct(m_Tag);
		StagePanel::GetInstance()->DeletePanel();
		m_Alive = false;
	}
}

void InterAction::Spawn() {
	float Psca = Ease(Out, Elastic, m_VanishFrame, 0.f, 0.15f);
	float sca = Ease(Out, Elastic, m_VanishFrame, 0.f, 0.5f);
	m_PannelScale = { Psca,Psca,Psca };
	m_Scale = { sca,sca,sca };
	m_Rotation.y += 3.0f;

	m_Pannel->SetPosition({ m_Position.x,m_Position.y + 0.1f,m_Position.z });
	m_Pannel->SetScale(m_PannelScale);
	m_Pannel->SetRotation(m_PannelRot);
	m_Pannel->SetColor(m_Color);
	m_Pannel->Update();
	if (Helper::GetInstance()->FrameCheck(m_VanishFrame, 1 / kVanishMax)) {
		_state = STATE_ALIVE;
		m_VanishFrame = 0;
	}


}

void InterAction::Alive() {
	{
		m_PannelRot.y += 0.3f;
		m_Pannel->SetPosition({ m_Position.x,m_Position.y + 0.1f,m_Position.z });
		m_Pannel->SetScale(m_PannelScale);
		m_Pannel->SetRotation(m_PannelRot);
		m_Pannel->SetColor(m_Color);
		m_Pannel->Update();
	}
}

void InterAction::Vanish() {

	float Psca = Ease(In,Elastic, m_VanishFrame,0.15f,0.f);
	float sca = Ease(In, Elastic, m_VanishFrame, 0.5f, 0.f);
	m_PannelScale = { Psca,Psca,Psca };
	m_Scale= { sca,sca,sca };
	m_PannelRot.y += 0.6f;
	m_Rotation.y += 3.0f;
	m_Pannel->SetRotation(m_PannelRot);
	m_Pannel->SetScale(m_PannelScale);
	m_Pannel->Update();
	if (Helper::GetInstance()->FrameCheck(m_VanishFrame, 1 / kVanishMax)) {
		m_Alive = false;
		m_VanishFrame = 0;
	}
}

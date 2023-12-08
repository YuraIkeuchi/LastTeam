#include "InterAction.h"
#include "Player.h"
#include "Collision.h"
#include "StagePanel.h"
#include <Easing.h>
#include <Helper.h>
#include <GameStateManager.h>
#include <SkillManager.h>
#include <TutorialTask.h>

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
	if (m_Delete) { return; }
	if (Collision::CircleCollision(
		m_Position.x, m_Position.z, m_Radius, 
		player->GetPosition().x,
		player->GetPosition().z, m_Radius)){

		//プレイヤーの行動数を増やしパネルを戻す
		GameStateManager::GetInstance()->AddSkill(m_SkillType,m_SkillID,m_Damage,m_Delay,m_Area,m_DistanceX,m_DistanceY,StateName);
		StagePanel::GetInstance()->DeletePanel();
		if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTHSKIL) {
			TutorialTask::GetInstance()->SetTutorialState(TASK_ATTACK);
		}
		m_Delete = true;
	}
}

void InterAction::Spawn() {
	float Psca = Ease(Out, Elastic, m_VanishFrame, 0.f, 0.1f);
	float sca = Ease(Out, Elastic, m_VanishFrame, 0.f, 0.3f);
	m_PannelScale = { Psca,Psca,Psca };
	m_Scale = { sca,sca,sca };
	m_Rotation.y += 3.0f;

	m_Pannel->SetPosition({ m_Position.x,m_Position.y + 0.1f,m_Position.z });
	m_Pannel->SetScale(m_PannelScale);
	m_Pannel->SetRotation(m_PannelRot);
	m_Pannel->SetColor(m_Color);
	m_Pannel->Update();
	if (Helper::FrameCheck(m_VanishFrame, 1 / kVanishMax)) {
		_state = STATE_ALIVE;
		m_VanishFrame = 0;
	}


}

void InterAction::Alive() {
	{
		//m_PannelRot.y += 0.3f;
		m_Pannel->SetPosition({ m_Position.x,m_Position.y + 0.1f,m_Position.z });
		m_Pannel->SetScale(m_PannelScale);
		m_Pannel->SetRotation(m_PannelRot);
		m_Pannel->SetColor(m_Color);
		m_Pannel->Update();
	}
}

void InterAction::Vanish() {

	float Psca = Ease(In,Quad, m_VanishFrame,0.15f,0.f);
	float sca = Ease(In, Quad, m_VanishFrame, 0.3f, 0.f);
	
	m_Position.y = Ease(In, Quad, m_VanishFrame, 0.0f, -3.0f);
	m_PannelScale = { Psca,Psca,Psca };
	m_Scale= { sca,sca,sca };
	//m_PannelRot.y += 0.6f;
	m_Rotation.y += 3.0f;
	m_Pannel->SetRotation(m_PannelRot);
	m_Pannel->SetScale(m_PannelScale);
	m_Pannel->Update();
	if (Helper::FrameCheck(m_VanishFrame, 1 / kVanishMax)) {
		m_Discard = true;
		m_Delete = true;
		m_VanishFrame = 0;
	}
}

//スキルデータ取得
void InterAction::GetSkillData() {
	SkillManager::GetInstance()->GetSkillType(m_SkillType);
	if (m_SkillType == (int)SkillType::damege) {
		SkillManager::GetInstance()->GetAttackSkillData(m_Damage, m_Delay, m_Area, m_DistanceX, m_DistanceY, StateName);
	}
	else if (m_SkillType == (int)SkillType::buff) {
		SkillManager::GetInstance()->GetSpecialSkillDate(m_Delay,StateName);
	} else if (m_SkillType == (int)SkillType::specialDamage) {
		SkillManager::GetInstance()->GetAttackSkillData(m_Damage, m_Delay, m_Area, m_DistanceX, m_DistanceY, StateName);

	}
}
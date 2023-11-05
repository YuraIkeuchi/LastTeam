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
	&InterAction::Spawn,//�����̍���
	&InterAction::Alive,//�����̍���
	&InterAction::Vanish,//�����̍���
};

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
	auto player = GameStateManager::GetInstance()->GetPlayer();
	//���b�V�������肠��
	if (Collision::CircleCollision(
		m_Position.x, m_Position.z, m_Radius, 
		player.lock()->GetPosition().x,
		player.lock()->GetPosition().z, m_Radius) &&
		(m_Alive)){
		//�v���C���[�̍s�����𑝂₵�p�l����߂�
		GameStateManager::GetInstance()->AddSkill(m_SkillID,m_Damage,m_Delay,m_Area,m_DistanceX,m_DistanceY);
		StagePanel::GetInstance()->DeletePanel();
		if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTHSKIL) {
			TutorialTask::GetInstance()->SetTutorialState(TASK_ATTACK);
		}
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

	float Psca = Ease(In,Quad, m_VanishFrame,0.15f,0.f);
	float sca = Ease(In, Quad, m_VanishFrame, 0.5f, 0.f);
	
	m_Position.y = Ease(In, Quad, m_VanishFrame, 0.0f, -3.0f);
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

//�X�L���f�[�^�擾
void InterAction::GetSkillData() {
	SkillManager::GetInstance()->GetSkillData(m_Damage, m_Delay, m_Area,m_DistanceX,m_DistanceY);
}